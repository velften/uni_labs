#ifndef SEM3LAB3_PAIR_H
#define SEM3LAB3_PAIR_H

#include <iostream>
#include <utility> // Для std::swap, если необходимо

template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair() noexcept : first(T1()), second(T2()) {}

    Pair(const T1& first, const T2& second) noexcept
            : first(first), second(second) {}

    Pair(T1&& first, T2&& second) noexcept
            : first(std::move(first)), second(std::move(second)) {}

    Pair(const Pair<T1, T2>& other) noexcept
            : first(other.first), second(other.second) {}

    Pair(Pair<T1, T2>&& other) noexcept
            : first(std::move(other.first)), second(std::move(other.second)) {}

    Pair<T1, T2>& operator=(const Pair<T1, T2>& other) noexcept {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    Pair<T1, T2>& operator=(Pair<T1, T2>&& other) noexcept {
        if (this != &other) {
            first = std::move(other.first);
            second = std::move(other.second);
        }
        return *this;
    }

    bool operator==(const Pair<T1, T2>& other) const noexcept {
        return (first == other.first) && (second == other.second);
    }

    bool operator!=(const Pair<T1, T2>& other) const noexcept {
        return !(*this == other);
    }

    ~Pair() = default;
};

#endif // SEM3LAB3_PAIR_H