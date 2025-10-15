#pragma once
#include "../repository/TextFileRepository.h"
#include "UndoService.h"

class AdminService {
private:
    TrenchCoatRepository& repository;
    UndoService undoService;
public:
    AdminService& operator=(const AdminService& other);
    AdminService(TrenchCoatRepository& repository): repository{repository} {}

    std::vector<TrenchCoat> getAllCoats(){return this->repository.getAll();}
    /** Add a coat to the repository
     *
     * @param newCoat `TrenchCoat` object to add to the repository
     * @throws ServiceException if the coat has already been added
     */
    void addCoat(const TrenchCoat& newCoat);

    /** Remove a coat from the repository
     *
     * @param coat `TrenchCoat` object to remove
     * @throws ServiceException if the coat is not in the repository
     */
    void removeCoat(TrenchCoat& coat);

    /** Update either the price or the quantity of a coat
     *
     * @param oldCoat old coat in the repository
     * @param newAttributeValue new value for selected attribute
     * @param attributeType "quantity" or "price"
     * @throws ServiceException if the coat is not in the repository or the attribute type is invalid
     */
    void updateCoat(TrenchCoat& oldCoat, int newAttributeValue, std::string& attributeType);

    /** Update the photo linked with a coat
     *
     * @param oldCoat coat to update
     * @param newLink new photo link
     * @throws ServiceException if the coat is not in the repository
     */
    void updateLink(TrenchCoat& oldCoat, std::string newLink);
    /** Get the number of trench coats in the repository
         */
    int size() const {return this->repository.size();}

    void undo();
    void redo();

};

