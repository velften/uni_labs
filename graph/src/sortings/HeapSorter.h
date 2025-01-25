#ifndef SEM3LAB2_HEAPSORTER_H
#define SEM3LAB2_HEAPSORTER_H

#include "ISorter.h"
#include <algorithm>

template <typename T>
class HeapSorter : public ISorter<T> {
public:
    Sequence<T>* Sort(Sequence<T>* seq, int (*cmp)(const T&, const T&)) override {
        int n = seq->getLength();
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(seq, n, i, cmp);
        }
        for (int i = n - 1; i > 0; i--) {
            std::swap((*seq)[0], (*seq)[i]);
            heapify(seq, i, 0, cmp);
        }
        return seq;
    }
    ~HeapSorter()= default;

private:
    void heapify(Sequence<T>* seq, int n, int i, int (*cmp)(const T&, const T&)) {
        int largest = i;
        int left, right;

        while (true) {
            left = 2 * i + 1;
            right = 2 * i + 2;
            if (left < n && cmp((*seq)[left], (*seq)[largest]) > 0) {
                largest = left;
            }
            if (right < n && cmp((*seq)[right], (*seq)[largest]) > 0) {
                largest = right;
            }
            if (largest == i) {
                break;
            }
            std::swap((*seq)[i], (*seq)[largest]);
            i = largest;
        }
    }
};


#endif //SEM3LAB2_HEAPSORTER_H