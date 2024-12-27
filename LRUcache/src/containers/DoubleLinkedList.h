#ifndef SEM3LAB1_LIST_H
#define SEM3LAB1_LIST_H
#include <iostream>
#include <stdexcept>

template <typename T>
class DoubleLinkedList {
public:
    struct Node {
        T *data;
        Node* next;
        Node *prev;

        explicit Node(const T &item) : data(new T(item)), next(nullptr), prev(nullptr) {}

        ~Node() {
            delete data;
        }
    };

private:
    Node* head;
    Node* tail;
    int length;


    T *getData() const {
        if (length == 0) return nullptr;

        T *data = new T[length];
        Node *current = head;
        for (int i = 0; i < length; i++) {
            data[i] = *(current->data);
            current = current->next;
        }
        return data;
    }

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), length(0) {}

    [[nodiscard]] int getLength() const {
        return length;
    }

    T &get(int index) const {
        return *(getNode(index)->data);
    }

    T &getFirst() const {
        if (!head) {
            throw std::out_of_range("DoubleLinkedList is empty.");
        }
        return get(0);
    }

    T &getLast() const {
        if (!tail) {
            throw std::out_of_range("DoubleLinkedList is empty.");
        }
        return *(tail->data);
    }

    Node *getHead() const {
        return head;
    }

    Node *getTail() const {
        return tail;
    }

    Node *getNode(int index) const {
        if (index >= length || index < 0) {
            throw std::out_of_range("Index is out of range.");
        }

        Node *current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current;
    }

    DoubleLinkedList<T> *getSubsequence(int startIndex, int endIndex) const {
        if ((startIndex >= length || startIndex < 0) ||
            (endIndex >= length || endIndex < 0) ||
            startIndex > endIndex) {
            throw std::out_of_range("Index is out of range.");
        }

        auto *subSequence = new DoubleLinkedList;
        Node *current = getNode(startIndex);
        for (int i = startIndex; i <= endIndex; i++) {
            subSequence->append(*(current->data));
            current = current->next;
        }
        return subSequence;
    }

    DoubleLinkedList<T> *append(const T &item) {
        Node* newNode = new Node(item);
        if (!head) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++length;
        return this;
    }

    DoubleLinkedList<T> *prepend(const T &item) {
        Node* newNode = new Node(item);
        if (!head) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++length;
        return this;
    }

    void moveToFront(Node* node) {
        if (node == head) {
            return;
        }
        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        }
        if (node == tail) {
            tail = node->prev;
        }
        node->next = head;
        node->prev = nullptr;
        if (head) {
            head->prev = node;
        }
        head = node;
        if (!tail) {
            tail = head;
        }
    }

    DoubleLinkedList<T> *insertAt(const T &item, int index) {
        if (index > getLength() || index < 0) {
            throw std::out_of_range("Index is out of range.");
        }
        if (index == 0) {
            return prepend(item);
        }
        if (index == length) {
            return append(item);
        }

        Node *prevNode = getNode(index - 1);
        Node* newNode = new Node(item);

        newNode->next = prevNode->next;
        newNode->prev = prevNode;
        if (prevNode->next) {
            prevNode->next->prev = newNode;
        }
        prevNode->next = newNode;
        ++length;
        return this;
    }

    DoubleLinkedList<T> *concat(const DoubleLinkedList<T> &sequence) {
        if (sequence.getLength() == 0) {
            return this;
        }

        Node *current = sequence.getHead();
        while (current) {
            append(*(current->data));
            current = current->next;
        }
        return this;
    }

    int find(const T &value) const {
        int index = 0;
        Node *current = head;
        while (current) {
            if (*(current->data) == value) {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1;
    }

    DoubleLinkedList<T> *map(T (*function)(T)) {
        Node *current = head;
        while (current) {
            *(current->data) = function(*(current->data));
            current = current->next;
        }
        return this;
    }

    DoubleLinkedList<T>* del(int index) {
        if (index >= length || index < 0) {
            throw std::out_of_range("Index is out of range.");
        }

        if (index == 0) {
            Node* temp = head;
            head = head->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
            delete temp;
        } else {
            Node* target = getNode(index);
            if (target == tail) {
                tail = target->prev;
            }
            if (target->next) {
                target->next->prev = target->prev;
            }
            if (target->prev) {
                target->prev->next = target->next;
            }
            delete target;
        }

        --length;
        return this;
    }

    void remove(Node* node) {
        if (!node) {
            return;
        }
        if (node == head) {
            head = node->next;
        }
        if (node == tail) {
            tail = node->prev;
        }
        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        }
        delete node;
        --length;
    }

    T reduce(T (*function)(T, T), T startValue) const {
        Node *current = head;
        T result = startValue;
        while (current) {
            result = function(*(current->data), result);
            current = current->next;
        }
        return result;
    }

    DoubleLinkedList<T> *where(bool (*function)(T)) const {
        auto result = new DoubleLinkedList<T>;
        Node *current = head;
        while (current) {
            if (function(*(current->data))) {
                result->append(*(current->data));
            }
            current = current->next;
        }
        return result;
    }

    T &operator[](int index) const {
        return get(index);
    }

    bool operator==(const DoubleLinkedList<T> &sequence) const {
        if (length != sequence.getLength()) {
            return false;
        }

        Node *current1 = head;
        Node *current2 = sequence.getHead();
        while (current1 && current2) {
            if (*(current1->data) != *(current2->data)) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }
        return true;
    }

    bool operator!=(const DoubleLinkedList<T> &sequence) const {
        return !(*this == sequence);
    }

    ~DoubleLinkedList() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};
#endif