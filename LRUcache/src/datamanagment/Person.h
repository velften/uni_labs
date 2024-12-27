#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

class Person {
public:
    std::string firstName;
    std::string lastName;
    int age;

    Person() = default;

    Person(std::string fName, std::string mName, int age)
            : firstName(std::move(fName)), lastName(std::move(mName)), age(std::move(age)) {}

    bool operator>(const Person& other) const {
        return firstName > other.firstName;
    }

    bool operator<(const Person& other) const {
        return firstName < other.firstName;
    }

    bool operator==(const Person& other) const {
        return firstName == other.firstName &&
               age == other.age &&
               lastName == other.lastName;
    }

    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << person.firstName << " " << person.lastName << " " << person.age;
        return os;
    }
};

#endif // PERSON_H
