#include "TrenchCoatValidator.h"
ValidatorException::ValidatorException(): message{"Validation Exception"} {}

void TrenchCoatValidator::validateCoat(const TrenchCoat &coat) {
    std::string errors;
    if (coat.getName().empty())
        errors += std::string("Name must not be empty\n");
    for (auto& c: coat.getName())
        if (c != ' ' && !isalpha(c)) {
            errors += std::string("Name must contain letters and spaces only.\n");
        }
    if (coat.getSize() != 'S' && coat.getSize() != 'M' && coat.getSize() != 'L')
        errors += std::string("Size must be either S, M or L.\n");
    if (coat.getColor().empty())
        errors += std::string("Color must not be empty.\n");
    if (coat.getPrice() < 0)
        errors += std::string("Price must be a non-negative integer.\n");
    if (coat.getQuantity() < 0)
        errors += std::string("Quantity must be a non-negative integer\n");
    if (coat.getPhoto().empty())
        errors += std::string("Link to photo must not be empty.\n");

    if (!errors.empty())
        throw ValidatorException(errors);
}

void TrenchCoatValidator::validateSize(char size) {
    if (size != 'S' && size != 'M' && size != 'L')
        throw ValidatorException("Size must be either S, M or L\n");
}




