#pragma once
#include <string>
#include <exception>

class ServiceException: public std::exception {
private:
    std::string message;
public:
    ServiceException();
    explicit ServiceException(std::string message);
    const char* what() const noexcept override;
};
