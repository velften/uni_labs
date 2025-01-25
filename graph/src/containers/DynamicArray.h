#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "../ptrs/UnqPtr.h"
#include "./Sequence.h"
#include <iostream>
#include <utility>
#include <stdexcept>

template <typename T>
class DynamicArray : public Sequence<T> {
    UnqPtr<T[]> data{new T[1], 1};
    int capacity;
    int length;

    void resize() {
        if (capacity == 0) {
            capacity = 1;
        }
        capacity *= 2;
        try {
            UnqPtr<T[]> new_data(new T[capacity], capacity);
            for (size_t i = 0; i < length; ++i) {
                new_data[i] = std::move(data[i]);
            }
            data = std::move(new_data);
        } catch (const std::bad_alloc&) {
            throw std::runtime_error("Memory allocation failed during resize.");
        }
    }

public:
    DynamicArray() : capacity(1), length(0) {
        data.reset(new T[1], 1);
    }

    explicit DynamicArray(int cap) : capacity(cap), length(0) {
        if (cap <= 0) {
            throw std::invalid_argument("Capacity must be greater than zero.");
        }
        data.reset(new T[cap], cap);
    }

    DynamicArray(T* ptr, int cap, int len) {
        if (cap <= 0 || len < 0) {
            throw std::out_of_range("Capacity and length must be non-negative.");
        }
        if ((len > 0 && cap > 0 && ptr == nullptr) || (len == 0 && cap == 0 && ptr != nullptr)) {
            throw std::invalid_argument("Invalid parameters for dynamic array.");
        }
        data.reset(ptr, cap);
        capacity = cap;
        length = len;
    }

    DynamicArray(const DynamicArray& other) : capacity(other.capacity), length(other.length) {
        UnqPtr<T[]> new_data(new T[other.capacity], other.capacity);
        for (size_t i = 0; i < length; ++i) {
            new_data[i] = other.data[i];
        }
        data = std::move(new_data);
    }

    DynamicArray(DynamicArray&& other) noexcept
            : data(std::move(other.data)), capacity(other.capacity), length(other.length) {
        other.capacity = 0;
        other.length = 0;
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            if (capacity < other.length) {
                resize();
            }
            length = other.length;
            for (size_t i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            capacity = other.capacity;
            length = other.length;
            other.capacity = 0;
            other.length = 0;
        }
        return *this;
    }

    T& get(int index) const override {
        if (length == 0) {
            throw std::out_of_range("Array is empty.");
        }
        if (index < 0 || index >= length) {
            throw std::out_of_range("Index is out of range.");
        }
        return data[index];
    }

    T& getFirst() const override {
        if (empty()) {
            throw std::out_of_range("Array is empty.");
        }
        return data[0];
    }

    T& getLast() const override {
        if (empty()) {
            throw std::out_of_range("Array is empty.");
        }
        return data[length - 1];
    }

    bool empty() const {
        return length == 0;
    }

    [[nodiscard]] int getLength() const override {
        return length;
    }

    void clear() {
        data.reset(nullptr, 0);
        length = 0;
        capacity = 0;
    }

    Sequence<T>* getSubsequence(int startIndex, int endIndex) const override{
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex) {
            throw std::out_of_range("Invalid index range.");
        }

        auto* subSeq = new DynamicArray<T>(new T[endIndex - startIndex + 1], 2 * (endIndex - startIndex + 1), endIndex - startIndex + 1);
        for (size_t i = 0; i <= endIndex - startIndex; ++i) {
            subSeq->data[i] = data[i + startIndex];
        }
        return subSeq;
    }

    DynamicArray<T>* append(const T& item) override {
        if (length >= capacity) {
            resize();
        }
        data[length++] = item;
        return this;
    }

    DynamicArray<T>* prepend(const T& item) override {
        if (length >= capacity) {
            resize();
        }
        for (size_t i = length; i > 0; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[0] = item;
        ++length;
        return this;
    }

    Sequence<T>* insertAt(const T& item, int index) override {
        if (index < 0 || index > length) {
            throw std::out_of_range("Index is out of range.");
        }
        if (length >= capacity) {
            resize();
        }
        for (size_t i = length; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = item;
        ++length;
        return this;
    }

    int find(const T& value) const override {
        for (int i = 0; i < length; i++) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    }

    Sequence<T>* concat(const Sequence<T>& other) override {
        if((length == 0) && (other.getLength() == 0)){
            return this;
        }
        while (length + other.getLength() >= capacity){
            resize();
        }
        size_t old_size = length;
        length += other.getLength();
        for(size_t i = 0; i < other.getLength(); i++){
            data[i + old_size] = other.getData()[i];
        }
        return this;
    }

    DynamicArray<T>* map(T (*function)(T)) override {
        for (size_t i = 0; i < length; i++) {
            data[i] = function(data[i]);
        }
        return this;
    }

    Sequence<T>* delByIndex(int index) override {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Index is out of range.");
        }
        for (size_t i = index; i < length - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --length;
        return this;
    }

    T reduce(T (*function)(T, T), T startValue) const override {
        T result = startValue;
        for (size_t i = 0; i < length; i++) {
            result = function(data[i], result);
        }
        return result;
    }

    DynamicArray<T>* where(bool (*function)(T)) const override {
        auto* result = new DynamicArray<T>;
        for (size_t i = 0; i < length; i++) {
            if (function(data[i])) {
                result->append(data[i]);
            }
        }
        return result;
    }

    T& operator[](int index) const override {
        return get(index);
    }

    bool operator==(const Sequence<T>& sequence) const override {
        if (length != sequence.getLength()) {
            return false;
        }
        for (size_t i = 0; i < length; i++) {
            if (data[i] != sequence.getData()[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Sequence<T>& sequence) const override {
        return !(*this == sequence);
    }

    T* getData() const override {
        return data.get();
    }
};

#endif // DYNAMICARRAY_H
