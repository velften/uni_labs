#ifndef SEM3LAB3_HASHING_H
#define SEM3LAB3_HASHING_H

#include "Pair.h"
#include <functional>
#include "../ptrs/ShrdPtr.h"

namespace std {

    template <typename T1, typename T2>
    struct hash<Pair<T1, T2>> {
        std::size_t operator()(const Pair<T1, T2>& pair) const {
            std::hash<T1> hash1;
            std::hash<T2> hash2;
            return hash1(pair.first) ^ (hash2(pair.second) << 1);
        }
    };

    template <typename T>
    struct hash<ShrdPtr<T>> {
        std::size_t operator()(const ShrdPtr<T>& ptr) const {
            return std::hash<T*>()(ptr.get());
        }
    };

}

#endif // HASHFUNCTIONS_H


