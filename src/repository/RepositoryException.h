#pragma once
#include <exception>
#include <string>

class RepositoryException: public std::exception {
private:
    std::string message;
public:
    RepositoryException();
    explicit RepositoryException(const std::string &message);
    [[nodiscard]] const char* what() const noexcept override;
};
