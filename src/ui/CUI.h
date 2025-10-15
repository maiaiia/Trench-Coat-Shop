#pragma once
#include <iostream>
#include "../service/AdminService.h"
#include "../service/UserService.h"
#include "../domain/TrenchCoatValidator.h"
#include "UI.h"

class CUI: public UI{
public:
    explicit CUI(AdminService& adminService, UserService& userService, bool adminMode);
    void run() override;
private:
    void handleException(const std::exception& exception) const override;
    void handleSuccessfulOperation(const std::string &message) const override;
    void handleErrorMessage(const std::string &message) const override;

    TrenchCoat readValidCoat() override;
    TrenchCoat readCoatIdentifiers() override;
    std::string readAttributeType() override;
    std::string readNewValue(const std::string& attribute) override;

    void printShoppingMenu() const ;
    void displayBasket() const override;
    char readSize() const override;
    void runShop(std::vector<TrenchCoat>&) const override;

private:
    [[nodiscard]] std::string getUserOption() const;

    void displayMenuAdmin();
    void displayMenuUser() const ;
    void displayAllAdmin() const;

    void runAdmin();
    void runUser() const;
};
