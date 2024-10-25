
#ifndef SEM3LAB1_PERSON_H
#define SEM3LAB1_PERSON_H
#include <iostream>
#include <string>
class Person {
    std::string firstName;
    std::string middleName;
    std::string lastName;

public:
    Person() = default;

    Person(const std::string& firstName, const std::string& middleName, const std::string& lastName) {
        this->firstName = firstName;
        this->middleName = middleName;
        this->lastName = lastName;
    }

    const std::string& getFirstName() const {
        return firstName;
    }

    const std::string& getMiddleName() const{
        return middleName;
    }

    const std::string& getLastName() const {
        return lastName;
    }


    void setFirstName(const std::string& newFirstName) {
        firstName = newFirstName;
    }


    void setMiddleName(const std::string& newMiddleName) {
        this->middleName = newMiddleName;
    }

    void setLastName(const std::string& newLastName) {
        this->lastName = newLastName;
    }


    bool operator>(const Person& other) const {
        return this->firstName > other.firstName;
    }

    bool operator==(const Person& other) const {
        return ((this->firstName == other.firstName) && (this->middleName == other.middleName) && (this->lastName == other.lastName));
    }

    void showInfo() const {
        std::cout << " | FirstName: ";
        std::cout << firstName;
        std::cout << "\n | MiddleName: ";
        std::cout << middleName;
        std::cout << "\n | LastName: ";
        std::cout << lastName;
    }

};
#endif //SEM3LAB1_PERSON_H
