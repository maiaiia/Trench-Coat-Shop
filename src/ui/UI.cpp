#include "UI.h"

UI::UI(AdminService &adminService, UserService &userService, const bool adminMode):
adminService{adminService}, userService{userService}, adminMode{adminMode} {}

void UI::openLink(const std::string& link) {
    const std::string command = "open " + link;
    system(command.c_str());
}
void UI::addCoat()  {
    TrenchCoat newCoat;
    try {
        newCoat = readValidCoat();
    }
    catch (ValidatorException& exception) {
        this->handleException(exception);
        return;
    }
    catch (UIException& e) {
        this->handleException(e);
        return;
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
        return;
    }

    try {
        this->adminService.addCoat(newCoat);
        this->handleSuccessfulOperation("Coat added successfully!");
    }
    catch (ServiceException& exception) {
        this->handleException(exception);
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
    }

}
void UI::deleteCoat() {
    TrenchCoat newCoat;
    try {
        newCoat = this->readCoatIdentifiers();
    }
    catch (ValidatorException& exception) {
        this->handleException(exception);
        return;
    }
    catch (UIException& e) {
        this->handleException(e);
        return;
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
        return;
    }
    try {
        this->adminService.removeCoat(newCoat);
    }
    catch (ServiceException& exception) {
        this->handleException(exception);
        return;
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
        return;
    }
    this->handleSuccessfulOperation("Coat removed successfully!");
}
void UI::updateCoat() {
    TrenchCoat oldCoat;
    try {
        oldCoat = readCoatIdentifiers();
    }
    catch (ValidatorException& exception) {
        this->handleException(exception);
        return;
    }
    catch (UIException& e) {
        this->handleException(e);
        return;
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
        return;
    }
    std::string attribute;
    try {
        attribute = this->readAttributeType();
    }
    catch (UIException& e) {
        this->handleException(e);
        return;
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
        return;
    }

    if (attribute != "quantity" && attribute != "price" && attribute != "link") {
        this->handleErrorMessage("Invalid attribute");
        return;
    }
    std::string newValueStr;
    try {
        newValueStr = this->readNewValue(attribute);
    }
    catch (UIException& e) {
        this->handleException(e);
        return;
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
        return;
    }

    int newValue;
    if (attribute == "link") {
        try {
            this->adminService.updateLink(oldCoat, newValueStr);
            this->handleSuccessfulOperation( "Link updated successfully!");
        }
        catch (ServiceException& exception) {
            this->handleException(exception);
        }
        return;
    }

    try {
        newValue = std::stoi(newValueStr);
        if (newValue < 0)
            throw std::exception();
    }
    catch (std::exception& exception) {
        this->handleErrorMessage("New value must be a non-negative integer.");
        return;
    }
    try {
        this->adminService.updateCoat(oldCoat,newValue,attribute);
        this->handleSuccessfulOperation("Coat updated successfully!");
    }
    catch (ServiceException& exception) {
        this->handleException(exception);
    }

}

void UI::openBasket() const {
    if (!this->userService.getBasketSize()) {
        this->handleErrorMessage("Basket is empty.");
        return;
    }
    this->userService.openBasket();
}

void UI::displayBasketConsole() const {
    if (!this->userService.getBasketSize()) {
        this->handleErrorMessage("Basket is empty.");
        return;
    }
    this->displayBasket();
}

std::vector<TrenchCoat> UI::initializeShop() const {
    const char size = this->readSize();
    auto coats = this->userService.getCoatsBySize(size);
    return coats;
}

void UI::shop() const {
    std::vector<TrenchCoat> validCoats;
    try {
        validCoats = this->initializeShop();
    }
    catch (std::exception& exception) {
        this->handleException(exception);
        return;
    }
    catch (...) {
        this->handleErrorMessage("An unexpected error has occurred.");
        return;
    }

    this->runShop(validCoats);
    this->userService.saveBasket();
}

void UI::undo() const {
    try {
        this->adminService.undo();
    }
    catch (std::exception& e) {
        this->handleException(e);
        return;
    }
    this->handleSuccessfulOperation("Undo successful!");
}

void UI::redo() const {
    try {
        this->adminService.redo();
    }
    catch (std::exception& e) {
        this->handleException(e);
        return;
    }
    this->handleSuccessfulOperation("Redo successful!");
}
