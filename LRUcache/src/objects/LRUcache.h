#ifndef SEM3LAB3_LRUCACHE_H
#define SEM3LAB3_LRUCACHE_H

#include "../containers/HashTable.h"
#include "../containers/DoubleLinkedList.h"
#include "../containers/Pair.h"
#include <stdexcept>

template<typename TKey, typename TValue>
class LRUCache {
    using nodePtr = typename DoubleLinkedList<Pair<TKey, TValue>>::Node*;
    HashTable<TKey, nodePtr> cacheMap;
    DoubleLinkedList<Pair<TKey, TValue>> cacheLine;
    size_t capacity;

public:
    explicit LRUCache(size_t capacity)
            : cacheMap(), cacheLine(), capacity(capacity) {}

    TValue& get(const TKey& key) {
        if (cacheMap.containsKey(key)) {
            nodePtr node = cacheMap[key];
            cacheLine.moveToFront(node);
            return node->data->second;
        }
        throw std::logic_error("Cache miss.");
    }

    void put(const TKey& key, const TValue& value) {
        if (cacheMap.containsKey(key)) {
            nodePtr node = cacheMap[key];
            node->data->second = value;
            cacheLine.moveToFront(node);
        } else {
            if (cacheLine.getLength() >= capacity) {
                nodePtr lastNode = cacheLine.getTail();
                if (lastNode) {
                    TKey keyToRemove = lastNode->data->first;
                    cacheMap.remove(keyToRemove);
                    cacheLine.remove(lastNode);
                }
            }
            Pair<TKey, TValue> newPair(key, value);
            cacheLine.prepend(newPair);
            cacheMap.add(key, cacheLine.getHead());
        }
    }

    size_t getSize() const {
        return cacheLine.getLength();
    }

    class Iterator {
        typename DoubleLinkedList<Pair<TKey, TValue>>::Node* currentNode;

    public:
        explicit Iterator(typename DoubleLinkedList<Pair<TKey, TValue>>::Node* node)
                : currentNode(node) {}

        bool operator==(const Iterator& other) const {
            return currentNode == other.currentNode;
        }

        bool operator!=(const Iterator& other) const {
            return currentNode != other.currentNode;
        }

        const Pair<TKey, TValue>& operator*() const {
            return *(currentNode->data);
        }

        const Pair<TKey, TValue>* operator->() const {
            return &currentNode->data;
        }

        Iterator& operator++() {
            if (currentNode) {
                currentNode = currentNode->next;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
    };


    Iterator beginC() const {
        return Iterator(cacheLine.getHead());
    }

    Iterator endC() const {
        return Iterator(nullptr);
    }


};

#endif //SEM3LAB3_LRUCACHE_H