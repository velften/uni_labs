
#ifndef SEM3LAB2_ISORTER_H
#define SEM3LAB2_ISORTER_H

#include "../containers/Sequence.h"
template <typename T>
class ISorter{
public:
    virtual Sequence<T>* Sort(Sequence<T>* seq, int (*cmp)(const T&, const T&)) = 0;
    virtual ~ISorter() = 0;
};
template <typename T>
ISorter<T>::~ISorter() {}
#endif //SEM3LAB2_ISORTER_H
