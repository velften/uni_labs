#ifndef SEM2LAB3_STACK_H
#define SEM2LAB3_STACK_H
#include "DynamicArray.h"
#include <stdexcept>
#include <iostream>

template <typename T>
class Stack {
    DynamicArray<T> dynamicArray;

public:
    Stack() = default;
    ~Stack() = default;

    Stack(const DynamicArray<T>& container) : dynamicArray(container) {}
    Stack(DynamicArray<T>&& container) noexcept : dynamicArray(std::move(container)) {}

    Stack(const Stack& other) : dynamicArray(other.dynamicArray) {}
    Stack(Stack&& other) noexcept : dynamicArray(std::move(other.dynamicArray)) {}

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            dynamicArray = other.dynamicArray;
        }
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            dynamicArray = std::move(other.dynamicArray);
        }
        return *this;
    }

    const T& top() const {
        if (dynamicArray.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return dynamicArray.getLast();
    }

    T& top() {
        if (dynamicArray.empty()) throw std::runtime_error("Stack is empty");

        return dynamicArray.getLast();
    }

    void pop() {
        if (dynamicArray.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        dynamicArray.delByIndex(dynamicArray.getLength() - 1);
    }

    void push(const T& element) {
        dynamicArray.append(element);
    }

    void push(T&& element) {
        dynamicArray.append(std::move(element));
    }

    Stack reverse() const {
        Stack reversedStack;
        DynamicArray<T> tempContainer = dynamicArray;
        while (!tempContainer.empty()) {
            reversedStack.push(tempContainer.getLast());
            tempContainer.delByIndex(tempContainer.getLength() - 1);
        }
        return reversedStack;
    }

    size_t getSize() const {
        return dynamicArray.getLength();
    }


    Stack substack(size_t startIndex, size_t endIndex) const {
        if (endIndex >= dynamicArray.getLength() || startIndex > endIndex) {
            throw std::out_of_range("Index out of range");
        }
        Stack tempStack;
        for (size_t i = startIndex; i <= endIndex; ++i) {
            tempStack.push(dynamicArray[i]);
        }
        return tempStack;
    }

    bool empty() const {
        return dynamicArray.empty();
    }


};
#endif //SEM3LAB4_STACK_H
