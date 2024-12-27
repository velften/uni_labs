#ifndef SEM3LAB1_UNQPTR_H
#define SEM3LAB1_UNQPTR_H

#include "PtrException.h"

template<typename T>
class UnqPtr {
    T* ptr{nullptr};
public:
    explicit UnqPtr(T* ptr = nullptr): ptr(ptr) {}

    UnqPtr(const UnqPtr& other) = delete;
    UnqPtr& operator=(const UnqPtr& other) = delete;

    ~UnqPtr() {
        delete ptr;
    }

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.release()) {}

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            reset(other.release());
        }
        return *this;
    }

    T& operator*() const {
        if (!ptr) {
            throw PtrException("Trying to dereference nullptr");
        }
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    T* get() const {
        return ptr;
    }

    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* new_ptr = nullptr) {
        if (ptr != new_ptr) {
            delete ptr;
            ptr = new_ptr;
        }
    }
};

template<typename T>
class UnqPtr<T[]> {
    T* ptr{nullptr};
    size_t size{0};

public:
    explicit UnqPtr(T* ptr = nullptr, size_t size = 0) : ptr(ptr), size(size) {}

    UnqPtr(const UnqPtr& other) = delete;
    UnqPtr& operator=(const UnqPtr& other) = delete;

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr), size(other.size) {
        other.ptr = nullptr;
        other.size = 0;
    }

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            reset(other.ptr, other.size);
            other.ptr = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~UnqPtr() {
        delete[] ptr;
    }

    T& operator[](size_t index) const {
        if (!ptr) {
            throw PtrException("Trying to access nullptr array");
        } else if (index >= size) {
            throw std::out_of_range("Index in UnqPtr array is out of range.");
        }
        return ptr[index];
    }

    T* get() const {
        return ptr;
    }

    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        size = 0;
        return temp;
    }

    void reset(T* new_ptr = nullptr, size_t new_size = 0) {
        if (ptr != new_ptr) {
            delete[] ptr;
            ptr = new_ptr;
            size = new_size;
        }
    }

    size_t getSize() const {
        return size;
    }
};

#endif //SEM3LAB1_UNQPTR_H
