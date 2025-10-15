#pragma once
#include "../domain/TrenchCoat.h"
#include <exception>

class BasketException: public std::exception {
private:
    std::string message;
public:
    BasketException(): message{"Basket Exception"} {}
    explicit BasketException(std::string message): message{message} {}
    [[nodiscard]] const char* what() const noexcept override{return this->message.c_str();}
};

class ShoppingBasket {
protected:
    std::vector<TrenchCoat> data;
    int total=0;
public:
    [[nodiscard]] int getTotal() const {return this->total;}
    [[nodiscard]] int size() const {return static_cast<int>(this->data.size());}
    [[nodiscard]] int getCoatCount() const;
    [[nodiscard]] std::vector<TrenchCoat> getAll() {return this->data;}
    [[nodiscard]] TrenchCoat getCoat(int position) const;

    void add(TrenchCoat& coat);

};
