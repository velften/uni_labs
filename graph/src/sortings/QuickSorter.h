#ifndef SEM3LAB2_QUICKSORTER_H
#define SEM3LAB2_QUICKSORTER_H

#include "ISorter.h"
#include "../containers/Sequence.h"
#include <algorithm>

template <typename T>
class QuickSorter : public ISorter<T> {
public:
    Sequence<T>* Sort(Sequence<T>* seq, int (*cmp)(const T&, const T&)) override {
        if (!seq || seq->getLength() <= 1) {
            return seq;
        }
        quicksort(seq, 0, seq->getLength() - 1, cmp);
        return seq;
    }

    ~QuickSorter()= default;

private:
    // Выбор медианы из трех элементов для опорного элемента
    int medianOfThree(Sequence<T>* seq, int low, int high, int (*cmp)(const T&, const T&)) {
        int mid = low + (high - low) / 2;

        if (cmp(seq->get(low), seq->get(mid)) > 0) {
            std::swap(seq->get(low), seq->get(mid));
        }
        if (cmp(seq->get(low), seq->get(high)) > 0) {
            std::swap(seq->get(low), seq->get(high));
        }
        if (cmp(seq->get(mid), seq->get(high)) > 0) {
            std::swap(seq->get(mid), seq->get(high));
        }

        std::swap(seq->get(mid), seq->get(high - 1));
        return high - 1;
    }

    void quicksort(Sequence<T>* seq, int low, int high, int (*cmp)(const T&, const T&)) {
        if (low >= high) {
            return;
        }

        int pivotIndex = medianOfThree(seq, low, high, cmp);
        T pivotValue = seq->get(pivotIndex);

        int i = low;
        int j = high - 1;

        while (true) {
            // Проверка на выход за границы для i
            while (i < high && cmp(seq->get(++i), pivotValue) < 0);

            // Проверка на выход за границы для j
            while (j > low && cmp(seq->get(--j), pivotValue) > 0);

            if (i < j) {
                std::swap(seq->get(i), seq->get(j));
            } else {
                break;
            }
        }

        std::swap(seq->get(i), seq->get(high - 1)); // Возвращаем опорный элемент на место

        quicksort(seq, low, i - 1, cmp);
        quicksort(seq, i + 1, high, cmp);
    }


};

#endif // SEM3LAB2_QUICKSORTER_H
