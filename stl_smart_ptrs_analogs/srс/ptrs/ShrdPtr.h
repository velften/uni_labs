#ifndef SEM3LAB1_SHRDPTR_H
#define SEM3LAB1_SHRDPTR_H
#include <iostream>
#include "PtrException.h"
template <typename T>
struct ControlBlock {
    T* ptr {nullptr};
    size_t shared_ref_count {0};
    size_t weak_ref_count {0};

    ControlBlock(T* Object, size_t shared_count, size_t weak_count) : ptr(Object), shared_ref_count(shared_count), weak_ref_count(weak_count) {}

    ControlBlock& operator=(const ControlBlock& other) {
        if (other.ptr == nullptr || ptr == nullptr) {
            throw PtrException("Trying to assign nullptr");
        }
        if (this != &other) {
            ptr = other.ptr;
            shared_ref_count = other.shared_ref_count;
            weak_ref_count = other.weak_ref_count;
        }
        return *this;
    }

    bool operator==(const ControlBlock& other) const {
        if (other.ptr && ptr != nullptr) {
            throw PtrException("Trying to compare with nullptr");
        }
        return ptr == other.ptr;
    }

    bool operator!=(const ControlBlock& other) const {
        return *this != other;
    }

    ~ControlBlock(){
        delete ptr;
    }
};

template <typename T>
class WeakPtr;


template<typename T>
class ShrdPtr{
    ControlBlock<T>* current_obj {nullptr};
    friend class WeakPtr<T>;
public:
    explicit ShrdPtr(T* Object) {
        if (Object != nullptr) {
            current_obj = new ControlBlock(Object, 1, 0);
        }
    }


    ShrdPtr(const ShrdPtr& other) : current_obj(other.current_obj) {
        if (current_obj) {
            current_obj->shared_ref_count++;
        }
    }

    ShrdPtr(ShrdPtr&& other) noexcept : current_obj(other.current_obj) {
        other.current_obj = nullptr;
    }

    T& operator*() const {
        if (!current_obj || !current_obj->ptr) {
            throw PtrException("Trying to dereference null ShrdPtr");
        }
        return *(current_obj->ptr);
    }

    T* operator->() const {
        if (!current_obj || !current_obj->ptr) {
            throw PtrException("Trying to reach nullptr");
        }
        return current_obj->ptr;
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if(this != &other) {
            if (current_obj) {
                current_obj->shared_ref_count--;
                if (current_obj->shared_ref_count == 0) {
                    delete current_obj;
                }
            }
            current_obj = other.current_obj;
            if (current_obj) {
                current_obj->shared_ref_count++;
            }
        }
        return *this;
    }

    [[nodiscard]] size_t useCount() const {
        if (!current_obj) {
            return 0;
        }
        return current_obj->shared_ref_count;
    }

    [[nodiscard]] bool isUnique() const {
        if (!current_obj) {
            throw PtrException("Cannot check uniqueness of null ShrdPtr");
        }
        return current_obj->shared_ref_count == 1;
    }

    explicit operator bool() const {
        return current_obj && current_obj->ptr;
    }

    T* get() const {
        if (!current_obj || !current_obj->ptr) {
            throw PtrException("Trying to get nullptr");
        }
        return current_obj->ptr;
    }

    void reset(T* newObject = nullptr) {
        if (current_obj) {
                current_obj->shared_ref_count--;
            if (current_obj->shared_ref_count == 0 && current_obj->weak_ref_count == 0) {
                delete current_obj;
            }
            current_obj = nullptr;
        }
        if (newObject) {
            current_obj = new ControlBlock(newObject, 1, 0);
        }
    }

    void swap(ShrdPtr<T>& other) {
        if (!current_obj || !current_obj->ptr || !other.current_obj || !other.current_obj->ptr) {
            throw PtrException("Trying to swap with nullptr");
        }
            std::swap(current_obj, other.current_obj);
    }

    ~ShrdPtr() {
        if (current_obj) {
            current_obj->shared_ref_count--;
            //std::cout << "~shared worked" << std::endl;
            if (current_obj->shared_ref_count + current_obj->weak_ref_count == 0) {
                //std::cout << "~object deleted " << std::endl;
                delete current_obj;
            }
        }
    }
};


template<typename T>
class   WeakPtr {
    ControlBlock<T>* current_obj {nullptr};
public:
    explicit WeakPtr(ControlBlock<T>* obj) : current_obj(obj) {
        if (current_obj) {
            current_obj->weak_ref_count++;
        }
    }

    WeakPtr(const WeakPtr<T>& other) : current_obj(other.current_obj) {
        if (current_obj) {
            current_obj->weak_ref_count++;
        }
    }

    explicit WeakPtr(const ShrdPtr<T>& other) : current_obj(other.current_obj) {
        if (current_obj) {
            current_obj->weak_ref_count++;
        }
    }


    WeakPtr& operator=(const ShrdPtr<T>& other) {
        if (current_obj != other.current_obj) {
            if (current_obj) {
                current_obj->weak_ref_count--;
                if (current_obj->weak_ref_count == 0 && current_obj->shared_ref_count == 0) {
                    delete current_obj;
                }
            }
            current_obj = other.current_obj;
            current_obj->weak_ref_count++;
        }

        return *this;
    }

    WeakPtr& operator=(const WeakPtr<T>& other) {
        if (current_obj && (this != &other)) {
            current_obj->weak_ref_count--;
            current_obj = other.current_obj;
            if (current_obj) {
                current_obj->weak_ref_count++;
            }
        }
        return *this;
    }



    [[nodiscard]] size_t useCount() const {
        return current_obj ? current_obj->weak_ref_count : 0;
    }


    [[nodiscard]] bool expired() const {
        return !current_obj || !(current_obj->ptr);
    }

    ~WeakPtr() {
        if (current_obj) {
            current_obj->weak_ref_count--;
            //std::cout << "~weak worked" << std::endl;
            if (current_obj->shared_ref_count + current_obj->weak_ref_count == 0) {
                //std::cout << "~object deleted " << std::endl;
                delete current_obj;
            }
        }
    }
};

#endif
