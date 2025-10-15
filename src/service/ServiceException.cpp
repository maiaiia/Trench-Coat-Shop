#include "ServiceException.h"

ServiceException::ServiceException(): message{"Service Exception"}{}
ServiceException::ServiceException(std::string message): message{message} {}

const char *ServiceException::what() const noexcept {
    return this->message.c_str();
}

