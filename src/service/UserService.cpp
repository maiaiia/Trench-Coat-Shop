#include "UserService.h"

UserService &UserService::operator=(const UserService & other) {
    if (this == &other)
        return *this;
    this->basket=other.basket;
    this->repository=other.repository;
    return *this;
}

std::vector<TrenchCoat> UserService::getCoatsBySize(char size) const{
    auto filter = [size](TrenchCoat coat){return size=='X' || coat.getSize()==size;};
    std::vector<TrenchCoat> result, allCoats = this->getAllCoats();
    std::copy_if(allCoats.begin(), allCoats.end(), std::back_inserter(result), filter);
    if (result.empty())
        throw ServiceException("Size out of stock.\n");
    return result;
}

void UserService::addToBasket(TrenchCoat coat) {
    if (!coat.getQuantity())
        throw ServiceException("Coat is out of stock.\n");

    this->basket.add(coat);
    coat.setQuantity(coat.getQuantity()-1);
    this->repository.updateCoat(coat, coat);
}

TrenchCoat UserService::getBasketItem(int position) {
    try {
        return this->basket.getCoat(position);
    }
    catch (BasketException&) {
        throw ServiceException("Invalid position.\n");
    }
}
