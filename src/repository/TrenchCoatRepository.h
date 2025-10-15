#pragma once
#include "../domain/TrenchCoat.h"
#include <vector>
#include <algorithm>
#include "RepositoryException.h"
#include "../src/domain/TrenchCoatValidator.h"

class TrenchCoatRepository {
public:
    virtual ~TrenchCoatRepository() = default;
    virtual bool isInRepo(const TrenchCoat& coat)=0;
    virtual int size() const = 0;
    virtual std::vector<TrenchCoat> getAll() = 0;

    /** Get a coat from the repository
     *
     * @param name - name of the coat
     * @param size - size of the coat
     * @param color - color of the coat
     * @return
     * @throws RepositoryException if the coat is not found
     */
    virtual TrenchCoat getCoat(std::string name, char size, std::string color) = 0;

    /** Add a new coat to the repository
     *
     * @param coat - `TrenchCoat` object to be added
     */
    virtual void addCoat (const TrenchCoat &coat) = 0;

    /** Remove a coat from the repository
     *
     * @param coat - `TrenchCoat` object to be removed
     * @throws RepositoryException if the coat is not in the repository
     */
    virtual void removeCoat (const TrenchCoat &coat) = 0;

    /** Set a new value for a coat
     *
     * @param oldCoat - `TrenchCoat` object to be updated
     * @param newCoat - `TrenchCoat` object representing the modified coat
     * @throws RepositoryException if `oldCoat` is not in the repository
     */
    virtual void updateCoat(const TrenchCoat &oldCoat, const TrenchCoat &newCoat) = 0;
};

