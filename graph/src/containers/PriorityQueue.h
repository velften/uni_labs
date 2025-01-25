#ifndef SEM3LAB4_PRIORITYQUEUE_H
#define SEM3LAB4_PRIORITYQUEUE_H

#include "DynamicArray.h"

template <typename T>
class PriorityNode {
public:
    long nodePriority;
    T nodeItem;

    PriorityNode() : nodePriority(0), nodeItem(T()) {}
    PriorityNode(const long priority, const T& value) : nodePriority(priority), nodeItem(value) {}

    bool operator<(const PriorityNode<T>& other) const {
        return nodePriority < other.nodePriority;
    }

    bool operator>(const PriorityNode<T>& other) const {
        return nodePriority > other.nodePriority;
    }

    bool operator==(const PriorityNode<T>& other) const {
        return nodePriority == other.nodePriority;
    }
};

template<typename T, typename Container = DynamicArray<PriorityNode<T>>, typename Comparator = std::greater<PriorityNode<T>>>
class PriorityQueue {
private:
    Container container;
    Comparator comparator;

    void heapifyUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (!comparator(container[parent], container[index])) {
                break;
            }
            std::swap(container[index], container[parent]);
            index = parent;
        }
    }

    void heapifyDown(size_t index) {
        size_t containerSize = container.getLength();
        while (true) {
            size_t largest = index;
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;

            if (left < containerSize && comparator(container[largest], container[left])) {
                largest = left;
            }
            if (right < containerSize && comparator(container[largest], container[right])) {
                largest = right;
            }

            if (largest == index) {
                break;
            }

            std::swap(container[index], container[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue() = default;
    explicit PriorityQueue(const Comparator& comp) : comparator(comp) {}

    PriorityQueue(const T* first, const T* last, const Comparator& comp = Comparator()) : comparator(comp) {
        while (first != last) {
            push(first->nodePriority, first->nodeItem);
            ++first;
        }
    }

    PriorityQueue(const PriorityNode<T>* nodes, size_t count, const Comparator& comp = Comparator()) : comparator(comp) {
        for (size_t i = 0; i < count; ++i) {
            container.append(nodes[i]);
        }

        if (container.getLength() > 1) {
            for (int i = (container.getLength() / 2) - 1; i >= 0; --i) {
                heapifyDown(i);
            }
        }
    }

    void swap(PriorityQueue<T, Container, Comparator>& other) noexcept {
        std::swap(container, other.container);
        std::swap(comparator, other.comparator);
    }

    bool operator==(const PriorityQueue<T, Container, Comparator>& other) const {
        if (container.getLength() != other.container.getLength()) return false;
        for (size_t i = 0; i < container.getLength(); ++i) {
            if (!(container[i] == other.container[i])) return false;
        }
        return true;
    }

    bool operator!=(const PriorityQueue<T, Container, Comparator>& other) const {
        return !(*this == other);
    }

    Comparator valueComp() const {
        return comparator;
    }

    bool empty() const noexcept {
        return container.empty();
    }

    size_t getSize() const noexcept {
        return container.getLength();
    }

    const PriorityNode<T>& top() const {
        if (empty()) throw std::runtime_error("Priority queue is empty");
        return container[0];
    }

    void push(const T& item, const long priority = 0) {
        container.append(PriorityNode<T>(priority, item));
        heapifyUp(container.getLength() - 1);
    }

    void changePriority(const long oldPriority, const long newPriority) {
        for (size_t i = 0; i < container.getLength(); ++i) {
            if (container[i].nodePriority == oldPriority) {
                container.append(PriorityNode<T>(newPriority, container[i].nodeItem));
                heapifyUp(container.getLength() - 1);
                container.delByIndex(i);
                heapifyDown(i);
                break;
            }
        }
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        if (container.getLength() == 1) {
            container.clear();
        } else {
            container[0] = container.getLast();
            container.delByIndex(container.getLength() - 1);
            heapifyDown(0);
        }
    }

    void merge(PriorityQueue<T, Container, Comparator>& other) {
        if (this == &other) return;
        for (size_t i = 0; i < other.getSize(); ++i) {
            container.append(other.container[i]);
        }

        for (int i = (container.getLength() / 2) - 1; i >= 0; --i) {
            heapifyDown(i);
        }
    }


    T extractMin() {
        if (empty()) throw std::runtime_error("Priority queue is empty");
        T result = container[0].nodeItem;
        pop();
        return result;
    }

    void clear() noexcept {
        container.clear();
    }

};

#endif // SEM3LAB4_PRIORITYQUEUE_