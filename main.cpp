#include "SQLiteRepository.h"
#include "src/ui/CUI.h"
#include "src/ui/GUI.h"
#include "tests.h"
#include <QtWidgets/QApplication>
#include "src/ui/models/BasketModel.h"

std::string selectRunMode() {
    std::string mode;
    while (true) {
        std::cout << "Enter app mode (user / admin): ";
        std::cin >> mode;
        if (mode == "user" || mode == "admin")
            return mode;
        std::cout << "Invalid mode.\n";
    }
}
std::string getBasketFormat() {
    std::string format;
    while (true) {
        std::cout << "Select basket file format (CSV / HTML): ";
        std::cin >> format;
        for (char & i : format) i = toupper(i);
        if (format == "CSV" || format == "HTML")
            return format;
        std::cout << "Invalid format.\n";
    }
}
std::string getDatabaseType() {
    std::string databaseType;
    while (true) {
        std::cout << "Select database type (text / sqlite): ";
        std :: cin >> databaseType;
        for (char & i: databaseType) i = toupper((i));
        if (databaseType == "TEXT" || databaseType == "SQLITE")
            return databaseType;
        std::cout << "Invalid type.\n";
    }
}

FileBasket* createBasket(std::string mode) {
    FileBasket* basket = nullptr;
    if (mode == "user") {
        const std::string format = getBasketFormat();
        if (format == "CSV") {
            basket = new CSVBasket("../basket_files/basket.csv");
        }
        else
            basket = new HTMLBasket("../basket_files/basket.html");
    }
    return basket;
}
TrenchCoatRepository* createRepository() {
    const std::string databaseType = getDatabaseType();
    const std::string basePath = "../databases/trench_coat_database";
    TrenchCoatRepository* repository = nullptr;
    if (databaseType == "TEXT")
        repository = new TextFileRepository(basePath + ".txt");
    else
        repository = new SQLiteRepository(basePath + ".db");
    return repository;
}
std::string selectUIType() {
    std::string uiType;
    while (1) {
        std::cout << "Enter UI type (CUI / GUI): ";
        std::cin >> uiType;
        for (auto& letter: uiType) letter = std::toupper(letter);
        if (uiType == "CUI" || uiType == "GUI")
            break;
        std::cout << "Invalid type./n";
    }
    return uiType;
}

int main(int argc, char* argv[]){
    testAll();
    std::cout << "All good!\n";
    //return 0;
    const std::string mode = selectRunMode();

    TrenchCoatRepository* repository = createRepository();
    FileBasket* basket = createBasket(mode);

    AdminService adminService = AdminService(*repository);
    UserService userService = UserService(*repository, *basket);
    auto model = BasketModel{*basket};

    std::string uiType = selectUIType();
    if (uiType == "CUI") {
        try {
            CUI ui = CUI(adminService, userService, mode == "admin");
            ui.run();
            delete basket;
            delete repository;
        }
        catch (...) {
            delete basket;
            delete repository;
        }
    }
    else if (uiType == "GUI") {
        try {
            QApplication application(argc, argv);
            GUI ui = GUI(adminService, userService, mode == "admin", model);
            ui.run();

            delete basket;
            delete repository;
        }
        catch (...) {
            delete basket;
            delete repository;
        }
    }

    return 0;
}
