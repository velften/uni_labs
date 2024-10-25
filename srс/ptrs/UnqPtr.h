#ifndef SEM3LAB1_UNQPTR_H
#define SEM3LAB1_UNQPTR_H
#include "PtrException.h"
    template<typename T>
    class UnqPtr {
        T* ptr {nullptr};
    public:
        explicit UnqPtr(T* ptr): ptr(ptr) {}

        UnqPtr(const UnqPtr& other) = delete;

        UnqPtr& operator=(const UnqPtr& other) = delete;

        ~UnqPtr(){
            delete ptr;
        }

        UnqPtr(UnqPtr&& other) noexcept {
            ptr = other.release();
            other.ptr = nullptr;
        }

        UnqPtr& operator=(UnqPtr&& other) noexcept {
            if (this != &other){
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }

        T& operator*() const {
            if(ptr == nullptr){
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

        void reset(T* new_ptr = nullptr){
            if (ptr != new_ptr) {
                T* old_ptr = ptr;
                ptr = new_ptr;
                delete old_ptr;
            }
        }
    };

template<typename T>
class UnqPtr<T[]> {
    T* ptr;
public:
    UnqPtr() : UnqPtr(nullptr) {}

    explicit UnqPtr(T* ptr): ptr(ptr) {}

    UnqPtr(const UnqPtr& other) = delete;

    UnqPtr& operator=(const UnqPtr& other) = delete;

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr){
        other.ptr = nullptr;
    }

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~UnqPtr() {
        delete[] ptr;
    }

    T& operator[](int index) const {
        if (ptr == nullptr) {
            throw PtrException("Trying to reach element from nullptr array");
        }
        else if (index < 0){
            throw std::out_of_range("Index is out of range");
        }
        return ptr[index];
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
            T* old_ptr = ptr;
            ptr = new_ptr;
            delete[] old_ptr;
        }
    }
};

#endif //SEM3LAB1_UNQPTR_H

