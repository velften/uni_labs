    #ifndef SEQUENCE_H
#define SEQUENCE_H
#include <iostream>

template<typename T>
class Sequence{
public:
    virtual ~Sequence() = default;

    virtual T& get(int index) const = 0;
    virtual T& getFirst() const = 0;
    virtual T& getLast() const = 0;
    virtual T* getData() const = 0;
    virtual int getLength() const = 0;
    virtual Sequence<T>* getSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T>* append(const T& item) = 0;
    virtual Sequence<T>* prepend(const T& item) = 0;
    virtual Sequence<T>* insertAt(const T& item, int index) = 0;
    virtual Sequence<T>* concat(const Sequence<T>& sequence) = 0;

    virtual int find(const T& value) const = 0;
    virtual Sequence<T>* map(T (*function)(T)) = 0;
    virtual Sequence<T>* delByIndex(int index) = 0;
    virtual T reduce(T (*function)(T, T), T startValue) const = 0;
    virtual Sequence<T>* where(bool (*function)(T)) const = 0;

    virtual T& operator[](int index) const = 0;
    virtual bool operator==(const Sequence<T>& sequence) const = 0;
    virtual bool operator!=(const Sequence<T>& sequence) const = 0;
};

#endif

