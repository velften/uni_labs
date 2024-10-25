
#ifndef SEM3LAB1_DYNAMICARRAY_H
#define SEM3LAB1_DYNAMICARRAY_H
#include "../ptrs/UnqPtr.h"
#include <iostream>
template <typename T>
class DynamicArray{
    UnqPtr<T[]> data;
    int capacity;
    int length;

    void resize() {
        capacity *= 2;
        UnqPtr<T[]> new_data(new T[capacity]);
        for (size_t i = 0; i < length; ++i) {
            new_data[i] = data[i];
        }
        data = std::move(new_data);

    }

    T* getData() const {
        return data.get();
    }
public:
    DynamicArray(): capacity(1), length(0){
        data.reset(new T[1]);
    }

    DynamicArray(T* ptr, int cap, int len){
        if (cap < 0){
            throw std::out_of_range("Capacity is negative number");
        }
        else if (len < 0){
            throw std::out_of_range("Length is negative number");
        }
        else if((len == 0 || cap == 0) && (!ptr)){
            throw "Null params for non null dynamic array";
        }
        data.reset(ptr);
        capacity = cap;
        length = len;
    }

    DynamicArray(const DynamicArray& other): capacity(other.capacity), length(other.length){
        T* new_data = new T[other.capacity];
        for (size_t i = 0; i < length; ++i){
            new_data[i] = other.data[i];
        }
        data.reset(new_data);
    }

    DynamicArray(DynamicArray&& other) noexcept
            : data(std::move(other.data)), capacity(other.capacity), length(other.length) {
        other.capacity = 0;
        other.length = 0;
    }

    DynamicArray& operator=(const DynamicArray& other){
        if (this != &other){
            if(capacity < other.length){
                this->resize();
            }
            length = other.length;
            for (size_t i = 0; i < length; ++i)
                data[i] = other.data[i];
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

    T& get(int index) const {
        if (length == 0) {
            throw "Array is empty.";
        }
        if (index < 0 || index > length - 1){
            throw std::out_of_range("Index is out of range.");
        }
        return data[index];
    }

    T& getFirst() const{
        return data[0];
    }
     T& getLast() const {
        return data[length-1];
    }
    [[nodiscard]] int getLength() const {
        return length;
    }

    DynamicArray<T>* getSubsequence(int startIndex, int endIndex) const{
        if (startIndex < 0 || endIndex > length - 1 || (startIndex == endIndex)){
            throw std::out_of_range("One of the indexes is out of range.");
        } 
        auto* subSeq = new DynamicArray<T>(new T[endIndex - startIndex + 1], 2* (endIndex - startIndex + 1),endIndex - startIndex + 1);
        for(size_t i = 0; i < endIndex - startIndex + 1; ++i){
            subSeq->data[i] = data[i + startIndex];
        }
        return subSeq;
    }

    DynamicArray<T>* append(const T& item) {
        if (length + 1 >= capacity){
            this->resize();
        }
        data[length] = item;
        ++length;
        return this;
    }

    DynamicArray<T>* prepend(const T& item) {
        if (length + 1 >= capacity){
            this->resize();
        }
        for(size_t i = length; i > 0; --i){
            data[i] = data[i - 1];
        }
        ++length;
        data[0] = item;
        return this;
    }

    DynamicArray<T>* insertAt(const T& item, int index) {
        if (index < 0 || index > length) {
            throw std::out_of_range("Index is out of range.");
        }
        if (length + 1 >= capacity) {
            resize();
        }
        for (size_t i = length; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = item;
        ++length;
        return this;
    }


    DynamicArray<T>* concat(const DynamicArray<T>& other) {
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

    int find(const T& value) const {
        for(int i = 0; i < length; i++){
            if(data[i] == value){
                return i;
            }
        }
        return -1;
    }
    DynamicArray<T>* map(T (*function)(T)) {
        for(size_t i = 0; i < length; i++) {
            data[i] = function(data[i]);
        }
        return this;
    }
    DynamicArray<T>* del(int index) {
                if(index >= length){
                    throw std::out_of_range("Index is out of range");
                }
                T* newData = new T[capacity];
                for (size_t i = 0; i < index; i++) {
                    newData[i] = data[i];
                }
                for(size_t i = index; i < length; i++){
                    newData[i] = data[i + 1];
                }
                length -= 1;
                data.reset(newData);
                return this;
        }

    T reduce(T (*function)(T, T), T startValue) const {
        T result = startValue;
        for(size_t i = 0; i < length; i++) {
            result = function(data[i], result);
        }
        return result;
    }
    DynamicArray<T>* where(bool (*function)(T)) const {
        auto* result = new DynamicArray<T>;
        for(size_t i = 0; i < length; i++){
            if(function(data[i])){
                result->append(data[i]);
            }
        }
        return result;
    }

    T& operator[](int index) const{
        return get(index);
    }

    bool operator==(const DynamicArray<T>& sequence) const {
        if(length != sequence.getLength()){
            return false;
        }
        T* data1 = getData();
        T* data2 = sequence.getData();
        for(size_t i = 0; i < length; i++){
            if(data1[i] != data2[i]){
                return false;
            }
        }
        return true;
    }

    bool operator!=(const DynamicArray<T>& sequence) const {
        return !(this->operator==(sequence));
    }

    ~DynamicArray() = default;
};



#endif //SEM3LAB1_DYNAMICARRAY_H
