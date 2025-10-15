#pragma once
#include <string>
#include <sstream>

//uniqueness criteria: name + size + color

class TrenchCoat {
private:
    char size;
    std::string name, color, photo;
    int price, quantity;
public:
    // --------- constructors ---------
    TrenchCoat(const std::string& name,char size, const std::string& color, int price, int quantity, const std::string& photo);
    TrenchCoat(const std::string& name, char size, const std::string& color);
    TrenchCoat(const TrenchCoat& other);
    TrenchCoat();

    // ----------- special methods ----------
    [[nodiscard]] std::string toCSV() const;
    [[nodiscard]] std::string toHTML() const;

    // ---------- operators ------------
    TrenchCoat& operator=(const TrenchCoat& other);
    bool operator<(const TrenchCoat & other) const;
    bool operator==(const TrenchCoat &) const;
    friend std::ostream& operator<<(std::ostream& output_stream, const TrenchCoat& coat);
    friend std::istream& operator>>(std::istream& input_stream, TrenchCoat& coat);

    // ------------ getters ------------
    [[nodiscard]] char getSize() const { return this->size; }
    [[nodiscard]] std::string getName() const { return this->name; }
    [[nodiscard]] int getPrice() const { return this->price; }
    [[nodiscard]] int getQuantity() const { return this->quantity; }
    [[nodiscard]] std::string getColor() const { return this->color; }
    [[nodiscard]] std::string getPhoto() const { return this->photo; }

    // ------------ setters --------------
    void setPrice(const int new_price) { this->price = new_price; }
    void setQuantity(const int new_quantity) { this->quantity = new_quantity; }
    void setLink(const std::string newLink) {this->photo = newLink;}

    void setName(const std::string newName) {this->name = newName;}
    void setColor(const std::string newColor) {this->color = newColor;}
    void setSize (const char newSize) {this->size = newSize;}
};
