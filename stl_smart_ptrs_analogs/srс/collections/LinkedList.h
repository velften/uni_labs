#ifndef SEM3LAB1_LIST_H
#define SEM3LAB1_LIST_H
#include "../ptrs/UnqPtr.h"
#include <iostream>

template <typename T>
class LinkedList {
private:
    struct Node {
        T *data;
        UnqPtr<Node> next;
        Node *prev;

        explicit Node(const T &item) : data(new T(item)), next(nullptr), prev(nullptr) {}

        ~Node() {
            delete data;
        }
    };

    UnqPtr<Node> head;

    Node* tail;

    int length;

    T *getData() const {
        T *data = new T[length];
        Node *current = head.get();
        for (size_t i = 0; i < length; i++) {
            data[i] = *(current->data);
            current = current->next.get();
        }
        return data;
    }


public:
    LinkedList() : head(nullptr), tail(nullptr), length(0) {}


    [[nodiscard]] int getLength() const {
        return length;
    }

    T &get(int index) const {
        return *(getNode(index)->data);
    }

    T &getFirst() const {
        return get(0);
    }

    T &getLast() const {
        return *(tail->data);
    }

    Node *getHead() const {
        return head.get();
    }

    Node *getNode(int index) const {
        if (index >= length || index < 0) {
            throw std::out_of_range("Index is out of range");
        }

        Node *current = head.get();
        for (size_t i = 0; i < index; i++) {
            current = current->next.get();
        }
        return current;
    }

    LinkedList<T> *getSubsequence(int startIndex, int endIndex) const {
        if ((startIndex >= length || startIndex < 0) || (endIndex >= length || endIndex < 0)) {
            throw std::out_of_range("Index is out of range");
        }

        auto *subSequence = new LinkedList;
        T *data = getData();
        for (size_t i = startIndex; i <= endIndex; i++) {
            subSequence->append(data[i]);
        }
        delete[] data;
        return subSequence;
    }

    LinkedList<T> *append(const T &item) {
        UnqPtr<Node> newNode(new Node(item));
        if (!head.get()) {
            head = std::move(newNode);
            tail = head.get();
        } else {
            tail->next = std::move(newNode);
            tail->next->prev = tail;
            tail = tail->next.get();
        }
        ++length;
        return this;
    }

    LinkedList<T> *prepend(const T &item) {
        auto newNode = UnqPtr<Node>(new Node(item));
        if (!head.get()) {
            tail = newNode.get();
        } else {
            head->prev = newNode.get();
        }
        newNode->next = std::move(head);
        head = std::move(newNode);
        ++length;
        return this;
    }

    LinkedList<T> *insertAt(const T &item, int index) {
        if (index > getLength() || index < 0) {
            throw std::out_of_range("Index is out of range");
        }
        if (index == 0) {
            return prepend(item);
        }
        if (index == length) {
            return append(item);
        }

        Node *prevNode = getNode(index - 1);
        auto newNode = UnqPtr<Node>(new Node(item));

        newNode->next = std::move(prevNode->next);
        if (newNode->next.get()) {
            newNode->next->prev = newNode.get();
        }

        prevNode->next = std::move(newNode);
        prevNode->next->prev = prevNode;

        ++length;
        return this;
    }

    LinkedList<T> *concat(const LinkedList<T> &sequence) {
        if (sequence.getLength() == 0) {
            return this;
        }
        T *sequenceData = sequence.getData();
        if (!head.get()) {
            head = UnqPtr<Node>(new Node(sequenceData[0]));
            tail = head.get();
            ++length;

            for (size_t i = 1; i < sequence.getLength(); i++) {
                auto newNode = UnqPtr<Node>(new Node(sequenceData[i]));
                tail->next = std::move(newNode);
                tail->next->prev = tail;
                tail = tail->next.get();
                ++length;
            }
        } else {
            for (size_t i = 0; i < sequence.getLength(); i++) {
                auto newNode = UnqPtr<Node>(new Node(sequenceData[i]));
                tail->next = std::move(newNode);
                tail->next->prev = tail;
                tail = tail->next.get();
            }
            length += sequence.getLength();
        }

        delete[] sequenceData;
        return this;
    }

    int find(const T &value) const {
        int index = 0;
        Node *current = head.get();
        while (current) {
            if (*(current->data) == value) {
                return index;
            }
            current = current->next.get();
            index++;
        }
        return -1;
    }

    LinkedList<T> *map(T (*function)(T)) {
        Node *current = head.get();
        while (current) {
            *(current->data) = function(*(current->data));
            current = current->next.get();
        }
        return this;
    }

    LinkedList<T>* del(int index) {
        if (index >= length || index < 0) {
            throw std::out_of_range("Index is out of range");
        }

        if (index == 0) {
            UnqPtr<Node> temp = std::move(head);
            head = std::move(temp->next);
            if (head.get()) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
        } else {
            Node* target = getNode(index);
            if (target == tail) {
                tail = target->prev;
            }
            if (target->next.get()) {
                target->next->prev = target->prev;
            }
            if (target->prev) {
                target->prev->next = std::move(target->next);
            }
        }

        --length;
        return this;
    }




    T reduce(T (*function)(T, T), T startValue) const {
        Node *current = head.get();
        T result = startValue;
        while (current) {
            result = function(*(current->data), result);
            current = current->next.get();
        }
        return result;
    }

    LinkedList<T> *where(bool (*function)(T)) const {
        auto result = new LinkedList<T>;
        Node *current = head.get();
        while (current) {
            if (function(*(current->data))) {
                result->append(*(current->data));
            }
            current = current->next.get();
        }
        return result;
    }

    T &operator[](int index) const {
        return get(index);
    }

    bool operator==(const LinkedList<T> &sequence) const {
        if (length != sequence.getLength()) {
            return false;
        }

        UnqPtr<T[]> data1(getData(), getLength());
        UnqPtr<T[]> data2(sequence.getData(), sequence.getLength());
        for (size_t i = 0; i < length; i++) {
            if (data1[i] != data2[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const LinkedList<T> &sequence) const {
        if (length != sequence.getLength()) {
            return true;
        }

        UnqPtr<T[]> data1(getData(), getLength());
        UnqPtr<T[]> data2(sequence.getData(), sequence.getLength());
        for (size_t i = 0; i < length; i++) {
            if (data1[i] != data2[i]) {
                return true;
            }
        }
        return false;
    }

    ~LinkedList() {
        while (head.get()) {
            UnqPtr<Node> temp = std::move(head);
            head = std::move(temp->next);
        }
    }
};
#endif