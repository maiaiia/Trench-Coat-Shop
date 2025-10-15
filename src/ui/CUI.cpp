#include "CUI.h"
const std::string ADMIN_ADD_COAT = "1", ADMIN_DELETE_COAT = "2", ADMIN_UPDATE_COAT = "3", ADMIN_DISPLAY_ALL = "4", ADMIN_UNDO = "5", ADMIN_REDO = "6", EXIT = "0";
const std::string USER_SHOPPING_MODE = "1", USER_DISPLAY_BASKET_CONSOLE = "2", USER_DISPLAY_BASKET_APP = "3", USER_BACK_TO_MENU = "b", USER_PREVIOUS_COAT = "p", USER_NEXT_COAT = "n", USER_ADD_TO_BASKET = "a";

CUI::CUI(AdminService &adminService, UserService &userService, bool adminMode): UI{adminService, userService, adminMode}{}

std::string CUI::readAttributeType() {
    std::cout << "Which attribute would you like to update? (enter price / quantity / link): ";
    std::string attribute; getline(std::cin, attribute);
    return attribute;
}
std::string CUI::readNewValue(const std::string& attribute) {
    std::string newValueStr;
    std::cout << "Enter new " << attribute << ": ";
    getline(std::cin, newValueStr);
    return newValueStr;
}
std::string CUI::getUserOption() const {
    std::string option;
    std::cout << "Enter an option: ";
    std::cin >> option;
    return option;
}
TrenchCoat CUI::readValidCoat() {
    std::string name, color, photo, sizeStr, quantityStr, priceStr;
    char size;
    int quantity, price;

    getline(std::cin, name);
    std::cout << "Enter coat name: ";
    getline(std::cin, name);
    std::cout << "Enter the size of the coat: ";
    getline(std::cin, sizeStr);
    sizeStr[0] = std::toupper(sizeStr[0]);
    size = sizeStr[0];
    std::cout << "Enter coat color: ";
    getline(std::cin, color);
    std::cout << "Enter the price of the coat: ";
    getline(std::cin, priceStr);
    try {
        price = std::stoi(priceStr);
    }
    catch (...){
        price = -1;
    }
    std::cout << "Enter the quantity in stock: ";
    getline(std::cin, quantityStr);
    try {
        quantity = std::stoi(quantityStr);
    }
    catch (...){
        quantity = -1;
    }
    std::cout << "Enter the photo of the coat: ";
    getline(std::cin, photo);

    TrenchCoat newCoat = TrenchCoat{name, size, color, price, quantity, photo};
    TrenchCoatValidator::validateCoat(newCoat);
    return newCoat;
}
TrenchCoat CUI::readCoatIdentifiers() {
    std::string name, sizeStr, color;

    getline(std::cin, name);
    std::cout << "Enter coat name: ";
    getline(std::cin, name);

    std::cout << "Enter the size of the coat: ";
    getline(std::cin, sizeStr);
    sizeStr[0] = std::toupper(sizeStr[0]);
    char size = sizeStr[0];

    std::cout << "Enter coat color: ";
    getline(std::cin, color);

    TrenchCoat newCoat = TrenchCoat{name, size, color};
    TrenchCoatValidator::validateCoat(newCoat);
    return newCoat;
}
char CUI::readSize() const {
    std::string sizeStr;
    getline(std::cin, sizeStr);
    std::cout << "Enter desired coat size (leave empty for all sizes): ";
    getline(std::cin, sizeStr);
    if (sizeStr.empty())
        return 'X';
    sizeStr[0] = std::toupper(sizeStr[0]);
    TrenchCoatValidator::validateSize(sizeStr[0]);
    return sizeStr[0];
}
void printCoatShoppingMode(const TrenchCoat& trenchCoat) {
    std::cout << "------- " << trenchCoat.getName() << " -------\n";
    std::cout << "Info:\n";
    std::cout << "\tSize: " <<  trenchCoat.getSize() << '\n';
    std::cout << "\tColor: " << trenchCoat.getColor() << '\n';
    std::cout << "\tPrice: " << trenchCoat.getPrice() << '\n';
    std::cout << "\tQuantity: " << trenchCoat.getQuantity() << '\n';
}
void printCoatBasketMode(const TrenchCoat& trenchCoat) {
    std::cout << trenchCoat.getName() << " - " << trenchCoat.getSize() << " - "<< trenchCoat.getColor() << " - " << trenchCoat.getPrice() << " - " << trenchCoat.getQuantity() << '\n';
}
void CUI::displayMenuAdmin() {
    std::cout << "Menu" << '\n';
    std::cout << ADMIN_ADD_COAT << ". Add a new coat\n";
    std::cout << ADMIN_DELETE_COAT << ". Remove a coat that is out of stock\n";
    std::cout << ADMIN_UPDATE_COAT << ". Update a coat\n";
    std::cout << ADMIN_DISPLAY_ALL << ". Display all coats\n";
    std::cout << ADMIN_UNDO << ". Undo\n";
    std::cout << ADMIN_REDO << ". Redo\n";
    std::cout << EXIT << ". Exit\n";
}
void CUI::printShoppingMenu() const {
    std::cout << "Previous ("  <<USER_PREVIOUS_COAT << ")       Next(" << USER_NEXT_COAT <<  ")       Add to basker ("<< USER_ADD_TO_BASKET<<")     ";
    std::cout << "Back to menu (" << USER_BACK_TO_MENU << ")\n";
}
void CUI::displayBasket() const {
    std::cout << "-------- Basket --------\n";
    std::cout << "Name - Size - Color - Price - Quantity\n";
    for (const auto &coat: this->userService.getBasket())
        printCoatBasketMode(coat);
    std::cout << "Total: " <<  this->userService.getBasketTotal() << "\n\n";
}
void CUI::displayMenuUser() const {
    std::cout << "Menu" << '\n';
    std::cout << USER_SHOPPING_MODE << ". Shop for trench coats\n";
    std::cout << USER_DISPLAY_BASKET_CONSOLE << ". See shopping basket in the console\n";
    std::cout << USER_DISPLAY_BASKET_APP << ". Open shopping basket\n";
    std::cout << EXIT << ". Exit\n";
}
void CUI::displayAllAdmin() const {
    std::cout << "Trench Coats\n";
    std::cout << "NAME | SIZE | COLOR | PRICE | QUANTITY | PHOTO\n";
    for (const auto &coat : this->adminService.getAllCoats())
        std::cout << coat << '\n';
}

void CUI::handleException(const std::exception &exception) const {
    std::cout << exception.what();
}
void CUI::handleSuccessfulOperation(const std::string &message) const {
    std::cout << message << '\n';
}
void CUI::handleErrorMessage(const std::string &message) const {
    std::cout << message << '\n';
}

void CUI::runShop(std::vector<TrenchCoat>& validCoats) const {
    int currentCoatIndex = 0;
    while (true) {
        TrenchCoat currentCoat = validCoats[currentCoatIndex];
        printCoatShoppingMode(currentCoat);
        //UI::openLink(currentCoat.getPhoto());
        this->printShoppingMenu();
        std::string userInput = this->getUserOption();
        if (userInput == USER_BACK_TO_MENU)
            return;
        if (userInput == USER_PREVIOUS_COAT)
            currentCoatIndex = (currentCoatIndex - 1 + static_cast<int>(validCoats.size())) % static_cast<int>(validCoats.size());
        else if (userInput == USER_NEXT_COAT)
            currentCoatIndex = (currentCoatIndex + 1) % static_cast<int>(validCoats.size());
        else if (userInput == USER_ADD_TO_BASKET) {
            try {
                this->userService.addToBasket(currentCoat);
                validCoats[currentCoatIndex].setQuantity(currentCoat.getQuantity()-1);
                this->handleSuccessfulOperation("Item added to basket successfully!\nTotal: " + std::to_string(this->userService.getBasketTotal()));
            }
            catch (ServiceException& exception) {
                this->handleException(exception);
            }
        }
        else
            this->handleErrorMessage("Invalid option");
    }
}


void CUI::runAdmin() {
    while (true) {
        try {
            this->displayMenuAdmin();
            std::string option = this->getUserOption();
            if (option == EXIT)
                break;
            if (option == ADMIN_ADD_COAT)
                this->addCoat();
            else if (option == ADMIN_DELETE_COAT)
                this->deleteCoat();
            else if (option == ADMIN_UPDATE_COAT)
                this->updateCoat();
            else if (option == ADMIN_DISPLAY_ALL)
                this->displayAllAdmin();
            else if (option == ADMIN_UNDO)
                this->undo();
            else if (option == ADMIN_REDO)
                this->redo();
            else
                this->handleErrorMessage("Invalid option.");
        }
        catch (std::exception& exception) {
            this->handleException(exception);
        }
        catch (...) {
            this->handleErrorMessage("An exception has occurred.");
        }
    }
}
void CUI::runUser() const {
    while (true) {
        this->displayMenuUser();
        std::string option = this->getUserOption();
        if (option == EXIT)
            break;
        if (option == USER_SHOPPING_MODE) {
            this->shop();
        }
        else if (option == USER_DISPLAY_BASKET_CONSOLE)
            this->displayBasketConsole();
        else if (option == USER_DISPLAY_BASKET_APP)
            this->openBasket();
        else
            this->handleErrorMessage("Invalid option.");
    }
}

void CUI::run() {
    this->adminMode ? this->runAdmin() : this->runUser();
}


