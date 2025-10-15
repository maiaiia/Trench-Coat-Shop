#pragma once
#include "TrenchCoat.h"

class ValidatorException: public std::exception {
private:
    std::string message;
public:
    ValidatorException();
    explicit ValidatorException(const std::string &_message) {this->message = _message;};
    [[nodiscard]] const char* what() const noexcept override {return this->message.c_str();}
};

class TrenchCoatValidator {
public:
    static void validateCoat(const TrenchCoat& coat);
    static void validateSize(char size);
};
