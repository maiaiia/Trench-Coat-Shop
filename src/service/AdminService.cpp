#include "AdminService.h"
#include "ServiceException.h"

AdminService &AdminService::operator=(const AdminService &other) {
    this->repository = other.repository;
    return *this;
}

void AdminService::addCoat(const TrenchCoat& newCoat) {
    if (this->repository.isInRepo(newCoat))
        throw ServiceException("Coat already in repository.\n");
    this->repository.addCoat(newCoat);
    this->undoService.addAction(std::make_unique<ActionAdd>(this->repository, newCoat));
}
void AdminService::removeCoat(TrenchCoat& coat) {
    try {
        coat = this->repository.getCoat(coat.getName(), coat.getSize(), coat.getColor());
    }
    catch (RepositoryException&) {
        throw ServiceException("Coat not found\n");
    }

    if (coat.getQuantity()!= 0)
        throw ServiceException("Item must be out of stock in order to be removed.\n");
    this->repository.removeCoat(coat);
    this->undoService.addAction(std::make_unique<ActionRemove>(this->repository, coat));
}
void AdminService::updateCoat(TrenchCoat &oldCoat, int newAttributeValue, std::string &attributeType) {
    try {
        oldCoat = this->repository.getCoat(oldCoat.getName(), oldCoat.getSize(), oldCoat.getColor());
    }
    catch (RepositoryException&) {
        throw ServiceException("Coat not found.\n");
    }
    TrenchCoat newCoat = TrenchCoat{oldCoat};
    if (attributeType == "quantity")
        newCoat.setQuantity(newAttributeValue);
    else if (attributeType == "price")
        newCoat.setPrice(newAttributeValue);
    else
        throw ServiceException("Invalid attribute to update.\n");
    this->repository.updateCoat(oldCoat,newCoat);
    this->undoService.addAction(std::make_unique<ActionUpdate>(this->repository, oldCoat, newCoat));
}

void AdminService::updateLink(TrenchCoat &oldCoat, std::string newLink) {
    try {
        oldCoat = this->repository.getCoat(oldCoat.getName(), oldCoat.getSize(), oldCoat.getColor());
    }
    catch (RepositoryException&) {
        throw ServiceException("Coat not found.\n");
    }
    TrenchCoat newCoat = TrenchCoat{oldCoat};
    newCoat.setLink(newLink);
    this->repository.updateCoat(oldCoat, newCoat);
    this->undoService.addAction(std::make_unique<ActionUpdate>(this->repository, oldCoat, newCoat));
}

void AdminService::undo() {
    this->undoService.undo();
}

void AdminService::redo() {
    this->undoService.redo();

}

