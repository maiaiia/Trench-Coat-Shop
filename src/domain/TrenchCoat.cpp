#include "TrenchCoat.h"
TrenchCoat::TrenchCoat(const std::string& name, char size, const std::string& color, int price, int quantity, const std::string& photo):
size{size}, name{name}, color{color}, photo{photo}, price{price}, quantity{quantity}{}
TrenchCoat::TrenchCoat(const std::string& name, char size, const std::string& color): size{size},name{name},color{color},photo{"empty"},price{0},quantity{0}{};
TrenchCoat::TrenchCoat() {
    this->size = '0';
    this->name = "";
    this->color = "";
    this->price = 0;
    this->quantity = 0;
    this->photo = "";
}
std::string TrenchCoat::toCSV() const {
    std::string result = this->name + ",";
    result.push_back(this->size);
    result += "," + this->color + "," + std::to_string(this->price) + "," + std::to_string(this->quantity) + "," + this->photo;
    return result;
}
std::string TrenchCoat::toHTML() const{
    std::string htmlFormat = "\t<tr>\n";
    htmlFormat += "\t\t<td><a href=\"" + this->photo + "\">" + this->name + "</a></td>\n";
    htmlFormat += "\t\t<td>";
    htmlFormat += this->size;
    htmlFormat += "</td>\n";
    htmlFormat += "\t\t<td>" + this->color + "</td>\n";
    htmlFormat += "\t\t<td>" + std::to_string(this->price) + "</td>\n";
    htmlFormat += "\t\t<td>" + std::to_string(this->quantity) + "</td>\n";
    htmlFormat += "\t</tr>\n";
    return htmlFormat;
}
TrenchCoat::TrenchCoat(const TrenchCoat& other) {
    this->size = other.getSize();
    this->name = other.getName();
    this->color = other.getColor();
    this->price = other.getPrice();
    this->quantity = other.getQuantity();
    this->photo = other.getPhoto();
}
TrenchCoat& TrenchCoat::operator=(const TrenchCoat& other) {
    if (this == &other)
        return *this;
    this->size = other.getSize();
    this->name = other.getName();
    this->color = other.getColor();
    this->price = other.getPrice();
    this->quantity = other.getQuantity();
    this->photo = other.getPhoto();
    return *this;
}
bool TrenchCoat::operator==(const TrenchCoat &other) const {
    return (this->name == other.getName() && this->size == other.getSize() && this->color == other.getColor());
}
bool TrenchCoat::operator<(const TrenchCoat & other) const {
    if (this->name != other.getName())
        return this->name < other.getName();

    if (this->size != other.getSize())
        return this->size > other.getSize();

    return this->color < other.getColor();

}

std::ostream& operator<<(std::ostream& output_stream, const TrenchCoat& coat) {
    output_stream << coat.toCSV();
    return output_stream;
}


std::istream& operator>>(std::istream& input_stream, TrenchCoat& coat) {
    std::string line; std::getline(input_stream, line);
    if (line.empty())
        return input_stream;
    std::istringstream lineStream(line);

    std::string name, sizeString, color, priceString, quantityString, photo;
    std::getline(lineStream, name, ',');
    std::getline(lineStream, sizeString, ',');
    std::getline(lineStream, color, ',');
    std::getline(lineStream, priceString, ',');
    std::getline(lineStream, quantityString, ',');
    std::getline(lineStream, photo, ',');

    int price = std::stoi(priceString);
    int quantity = std::stoi(quantityString);
    char size = sizeString[0];

    coat = TrenchCoat{name, size, color, price, quantity, photo};
    return input_stream;
}