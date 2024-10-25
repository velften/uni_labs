#ifndef SEM3LAB1_TIMER_H
#define SEM3LAB1_TIMER_H
#include <iostream>
#include <chrono>
/**
 * @brief Класс Timer для измерения времени выполнения методов.
 *
 * @tparam T Тип данных, с которыми работает метод.
 * @tparam D Шаблонный класс, содержащий методы для измерения времени.
 */
template <typename T, template <typename> class D>
class Timer {
    /**
     * Количество итераций для измерения времени.
     */
    size_t numIter;

    /**
     * Ссылка на объект класса D, содержащего методы для измерения времени.
     */
    D<T>& obj_;

public:
    /**
     * Конструктор класса Timer.
     *
     * @param obj Объект класса D, содержащего методы для измерения времени.
     * @param iter Количество итераций для измерения времени.
     */
    Timer(D<T>& obj, size_t iter) : obj_(obj), numIter(iter) {}

    /**
     * @brief Функтор для измерения времени выполнения метода.
     *
     * @tparam Method Тип метода, время выполнения которого необходимо измерить.
     * @param method Указатель на метод, время выполнения которого необходимо измерить.
     * @param item Объект, передаваемый в метод.
     * @return Возвращаемое значение метода.
     */
    template <typename Method>
     long long operator()(Method method, T& item) {
        if (!method) {
            throw "Method pointer is null";
        }
        auto start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numIter; ++i) {
            (obj_.*method)(item);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time_taken = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        return time_taken;
    }

    /**
  * @brief Функция-член для измерения времени выполнения метода STL.
  *
  * @tparam Method Тип метода, время выполнения которого необходимо измерить.
  * @param method Лямбда-функция, время выполнения которой необходимо измерить.
  * @attention Поскольку в STL практически все методы шаблонных классов перегружены, необходимо передавать лямбда-функцию
  * вместо указателя на метод шаблонного класса.
  */
    template <typename Method>
    long long STLmeasureTime(Method method) {
        auto start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numIter; ++i) {
            method();
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time_taken = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        return time_taken;
    }
};

#endif //SEM3LAB1_TIMER_H
