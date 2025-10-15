#pragma once
#include "../repository/TextFileRepository.h"
#include "ServiceException.h"
#include "../repository/FileBasket.h"

class UserService {
private:
    TrenchCoatRepository& repository;
    FileBasket& basket;
public:
    UserService(TrenchCoatRepository& repository, FileBasket& basket): repository{repository}, basket{basket} {}

    UserService & operator=(const UserService &);
    /** Get the total cost of the items in the basket
     */
    int getBasketTotal() const {return this->basket.getTotal();}
    /** Get the number of items in the basket
     */
    int getBasketSize() const {return this->basket.size();}
    /** Get the element at a certain position in the basket
     *
     * @param position position of the trench coat
     * @return `TrenchCoat` object
     * @throws ServiceException if the position is not valid
     */
    TrenchCoat getBasketItem(int position);
    void saveBasket() const {this->basket.writeToFile();}
    void openBasket() const {this->basket.displayBasket();}

    /** Get all coats with a certain size
     *
     * @param size - size of the coats to be selected
     * @throws ServiceException if there are no coats of the specified size
     * @return a vector with all coats of the specified size
     */
    std::vector<TrenchCoat> getCoatsBySize(char size) const;
    std::vector<TrenchCoat> getBasket() {return this->basket.getAll();}

    /** Add a coat to the basket
     *
     * @param coat coat to add
     * @throws ServiceException if the coat is out of stock
     */
    void addToBasket(TrenchCoat coat);
    /** Get the number of trench coats in the repository
    */
    int size() const {return this->repository.size();}

    std::vector<TrenchCoat> getAllCoats()const {return this->repository.getAll();}
};
