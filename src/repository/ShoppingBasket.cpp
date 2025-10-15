#include "ShoppingBasket.h"

int ShoppingBasket::getCoatCount() const {
    int count=0;
    for (const auto& coat : this->data)
        count += coat.getQuantity();
    return count;
}

TrenchCoat ShoppingBasket::getCoat(int position) const {
    try {
        return this->data.at(position);
    }
    catch (...) {
        throw BasketException("Invalid position.\n");
    }
}

void ShoppingBasket::add(TrenchCoat &coat) {
    auto pos = (std::find(this->data.begin(), this->data.end(), coat)-this->data.begin());
    if (pos == this->data.size())
        this->data.emplace_back(TrenchCoat{coat.getName(), coat.getSize(), coat.getColor(), coat.getPrice(), 0, coat.getPhoto()});
    TrenchCoat oldCoat = this->data.at(pos);
    oldCoat.setQuantity(oldCoat.getQuantity()+1);
    this->data.at(pos)=oldCoat;
    this->total += coat.getPrice();
}
