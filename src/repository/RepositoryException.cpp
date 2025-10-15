#include "RepositoryException.h"

RepositoryException::RepositoryException(): message{"Repository Exception"} {}

RepositoryException::RepositoryException(const std::string &message): message{message} {}

const char *RepositoryException::what() const noexcept {
    return this->message.c_str();
}
