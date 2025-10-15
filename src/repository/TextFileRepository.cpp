#include "TextFileRepository.h"

TextFileRepository::TextFileRepository(std::string filename) {
    this->filename=filename;
    this->loadDataFromFile();
}
TextFileRepository::TextFileRepository(TextFileRepository& other) {
    this->filename = other.filename;
    this->data = other.data;
}
TextFileRepository::~TextFileRepository() {
    this->saveDataToFile();
}
bool TextFileRepository::isInRepo(const TrenchCoat &coat) {
    return (this->getCoatPosition(coat) != -1);
}
void TextFileRepository::addCoat (const TrenchCoat &coat) {
    this->data.push_back(coat);
    std::sort(this->data.begin(), this->data.end());
    this->saveDataToFile();
}

TrenchCoat TextFileRepository::getCoat(std::string name, char size, std::string color) {
    TrenchCoat searchCoat = TrenchCoat(name, size, color);
    int position = this->getCoatPosition(searchCoat);
    if (position == -1)
        throw RepositoryException("Coat not found.");
    return this->data[position];
}

TrenchCoat TextFileRepository::getElement(const int position) {
    TrenchCoat element;
    try {
        element = this->data.at(position);
    }
    catch (...) {
        throw RepositoryException("Invalid position.\n");
    }
    return element;
}

int TextFileRepository::getCoatPosition(const TrenchCoat &coat) {
    const auto it = std::find(this->data.begin(), this->data.end(), coat);
    if (it == this->data.end())
        return -1;
    return static_cast<int>(it-this->data.begin());
}
void TextFileRepository::removeCoat (const TrenchCoat &coat) {
    int index = this->getCoatPosition(coat);
    if (index == -1)
        throw RepositoryException("Coat not in repository\n");
    this->data.erase(this->data.begin() + index);
    this->saveDataToFile();
}

void TextFileRepository::loadDataFromFile() {
    std::ifstream fin(this->filename);
    std::string line;
    TrenchCoat newCoat;
    while (fin >> newCoat) {
        if (newCoat == TrenchCoat{})
            break;
        this->addCoat(newCoat);
    }
    fin.close();
}
void TextFileRepository::saveDataToFile() const {
    if (this->filename.empty())
        return;
    std::ofstream fout(this->filename);

    for (const auto &coat: this->data)
        fout << coat << '\n';

    fout.close();
}

void TextFileRepository::updateCoat(const TrenchCoat &oldCoat, const TrenchCoat &newCoat) {
    int position = this->getCoatPosition(oldCoat);
    if (position == -1)
        throw RepositoryException("Invalid position.");
    this->data.at(position) = newCoat;
    this->saveDataToFile();
}