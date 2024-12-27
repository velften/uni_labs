#ifndef SEM3LAB3_HASHING_H
#define SEM3LAB3_HASHING_H

#include "Pair.h"
#include <functional>
#include "../datamanagment/Person.h"

namespace std {

    template <typename T1, typename T2>
    struct hash<Pair<T1, T2>> {
        std::size_t operator()(const Pair<T1, T2>& pair) const {
            std::hash<T1> hash1;
            std::hash<T2> hash2;
            return hash1(pair.first) ^ (hash2(pair.second) << 1);
        }
    };

    template <>
    struct hash<Person> {
        std::size_t operator()(const Person& person) const {
            std::hash<std::string> hashString;
            std::hash<int> hashInt;
            return hashString(person.firstName) ^ (hashInt(person.age) << 1);
        }
    };

}

#endif // HASHFUNCTIONS_H


