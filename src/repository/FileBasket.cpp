#include "FileBasket.h"
FileBasket::FileBasket(const std::string filename): filename{filename}{}
CSVBasket::CSVBasket(const std::string filename): FileBasket(filename) {}
HTMLBasket::HTMLBasket(const std::string filename):FileBasket(filename) {}

void HTMLBasket::writeToFile() {
    std::ofstream out(this->filename);
    out << "<!DOCTYPE html>\n<html>\n<head>\n\t<title>Basket</title>\n</head>\n<body>\n<table border=\"1\">\n";
    out << "\t<tr>\n\t\t<td>Coat</td>\n\t\t<td>Size</td>\n\t\t<td>Color</td>\n\t\t<td>Price</td>\n\t\t<td>Quantity</td>\n\t</tr>\n";
    for (const auto& coat: this->data)
        out << coat.toHTML();
    out << "</table>\n</body>\n</html>";
    out.close();
}
void HTMLBasket::displayBasket() {
    std::string command = "open " + this->filename;
    system(command.c_str());
}
void CSVBasket::writeToFile() {
    std::ofstream out(this->filename);
    for (const auto& coat: this->data)
        out << coat.toCSV() << '\n';
    out.close();
}
void CSVBasket::displayBasket() {
    std::string command = "open " + this->filename;
    system(command.c_str());
}
