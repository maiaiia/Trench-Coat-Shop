#pragma once
#include "ShoppingBasket.h"
#include <fstream>

class FileBasket: public ShoppingBasket {
protected:
    std::string filename;
public:
    explicit FileBasket(std::string filename);
    virtual ~FileBasket() = default;

    virtual void writeToFile() = 0;
    virtual void displayBasket() = 0;
};
class HTMLBasket: public FileBasket {
public:
    explicit HTMLBasket(std::string filename);

    void writeToFile() override;
    void displayBasket() override;
};
class CSVBasket: public FileBasket {
public:
    explicit CSVBasket(std::string filename);

    void writeToFile() override;
    void displayBasket() override;
};
