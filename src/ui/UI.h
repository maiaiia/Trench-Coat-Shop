#pragma once
#include "AdminService.h"
#include "UserService.h"

class UIException: public std::exception {
private:
    std::string message;
public:
    explicit UIException(std::string message = "UI Exception"):message{message}{}
    [[nodiscard]] const char *what() const noexcept override {return this->message.c_str();}
};

class UI {
protected:
    AdminService& adminService;
    UserService& userService;
    bool adminMode;
public:
    explicit UI(AdminService& adminService, UserService& userService, bool adminMode);
    virtual ~UI() = default;
    virtual void run()=0;

protected:
    virtual void handleException(const std::exception& exception) const = 0;

    virtual void handleErrorMessage(const std::string& message) const = 0;
    virtual void handleSuccessfulOperation(const std::string& message) const = 0;

    virtual TrenchCoat readValidCoat()=0;
    virtual TrenchCoat readCoatIdentifiers()=0;

    virtual std::string readAttributeType()=0;
    virtual std::string readNewValue(const std::string& attribute) = 0;

    static void openLink(const std::string& link) ;
    [[nodiscard]] virtual char readSize() const = 0;

    void addCoat();
    void deleteCoat();
    void updateCoat();
    void undo() const;
    void redo() const;

    virtual void displayBasket() const = 0;
    virtual void displayBasketConsole() const;
    void openBasket() const;

    void shop() const;
    virtual void runShop(std::vector<TrenchCoat>&) const = 0;
    [[nodiscard]] std::vector<TrenchCoat> initializeShop() const;
};
