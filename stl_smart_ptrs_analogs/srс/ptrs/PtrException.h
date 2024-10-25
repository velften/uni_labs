//
// Created by velften on 18/10/24.
//

#ifndef SEM3LAB1_PTREXCEPTION_H
#define SEM3LAB1_PTREXCEPTION_H
#include <exception>
#include <string>
#include <utility>
class PtrException: public std::exception
{
public:
    explicit PtrException(std::string  message): message{std::move(message)}{}

    [[nodiscard]] const char* what() const noexcept override
    {
        return message.c_str();
    }
private:
    std::string message;
};


#endif //SEM3LAB1_PTREXCEPTION_H
