#ifndef SEM2LAB3_QUEUE_H
#define SEM2LAB3_QUEUE_H
#include "Stack.h"
#include <stdexcept>
#include <iostream>

template <typename T>
class Queue {
private:
    Stack<T> inputStack;
    Stack<T> outputStack;

    void transferElements() {
        if (outputStack.empty()) {
            while (!inputStack.empty()) {
                outputStack.push(std::move(inputStack.top()));
                inputStack.pop();
            }
        }
    }

public:
    Queue() = default;
    ~Queue() = default;

    Queue(const Queue& other) : inputStack(other.inputStack), outputStack(other.outputStack) {}

    Queue(Queue&& other) noexcept
            : inputStack(std::move(other.inputStack))
            , outputStack(std::move(other.outputStack)) {}

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            inputStack = other.inputStack;
            outputStack = other.outputStack;
        }
        return *this;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            inputStack = std::move(other.inputStack);
            outputStack = std::move(other.outputStack);
        }
        return *this;
    }

    void push(const T& item) {
        inputStack.push(item);
    }

    void push(T&& item) {
        inputStack.push(std::move(item));
    }

    void dequeue() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        transferElements();
        outputStack.pop();
    }

    const T front() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        transferElements();
        return outputStack.top();
    }

    size_t getSize() const noexcept {
        return inputStack.getSize() + outputStack.getSize();
    }

    bool empty() const noexcept {
        return inputStack.empty() && outputStack.empty();
    }

    Queue reverse() const {
        Queue reversedQueue;
        Stack<T> tempInput = inputStack;
        Stack<T> tempOutput = outputStack;

        while (!tempOutput.empty()) {
            reversedQueue.push(tempOutput.top());
            tempOutput.pop();
        }

        while (!tempInput.empty()) {
            reversedQueue.push(tempInput.top());
            tempInput.pop();
        }

        return reversedQueue;
    }


};
#endif
