#pragma once
#include <fstream>
#include "TrenchCoatRepository.h"

class TextFileRepository: public TrenchCoatRepository {
private:
    std::vector<TrenchCoat> data;
    std::string filename;
public:
    /** Parameter constructor */
    explicit TextFileRepository(std::string filename);
    /** Copy constructor */
    TextFileRepository(TextFileRepository& other);
    /** Destructor */
    ~TextFileRepository() override;

    /** Get the number of elements stored in the repository
     */
    int size() const override {return this->data.size();}
    std::vector<TrenchCoat> getAll() override{return this->data;}

    bool isInRepo(const TrenchCoat& coat) override;
    void addCoat (const TrenchCoat &coat) override;
    void removeCoat (const TrenchCoat &coat) noexcept(false) override;
    void updateCoat(const TrenchCoat &oldCoat, const TrenchCoat &newCoat) override;

    TrenchCoat getCoat(std::string name, char size, std::string color) override;
private:
    /** Load a list of `TrenchCoat` objects from a file
     */
    void loadDataFromFile();
    /** Write the list of `TrenchCoat` objects to a file
     */
    void saveDataToFile() const;

    /** Return the element at a certain position
     *
     * @param position of the element
     * @return element at said position
     * @throws RepositoryException if the position is invalid
     */
    TrenchCoat getElement(const int position);

    /** Get the position of a coat in the repository
     *
     * @param coat - `TrenchCoat` object to be searched
     * @return The position of the coat if it is in the repository and -1 otherwise //
     */
    int getCoatPosition(const TrenchCoat &coat);
};
