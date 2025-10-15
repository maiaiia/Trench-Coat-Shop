#include "SQLiteRepository.h"

SQLiteRepository::SQLiteRepository(std::string filename): filename{filename} {
    if (sqlite3_open(filename.c_str(), &database) != SQLITE_OK)
        throw RepositoryException("Failed to open database");
    this->initializeDatabase();
}

void SQLiteRepository::initializeDatabase() {
    const char* sql = "CREATE TABLE IF NOT EXISTS trenchcoats (" \
        "name TEXT NOT NULL," \
        "size TEXT NOT NULL," \
        "color TEXT NOT NULL," \
        "price INTEGER NOT NULL," \
        "quantity INTEGER NOT NULL," \
        "link TEXT NOT NULL," \
        "PRIMARY KEY (name, size, color)" \
    ")";
    this->executeSQLNoCallback(sql);
}

void SQLiteRepository::executeSQLNoCallback(const std::string sql) const {
    char* errorMessage = nullptr;
    if (sqlite3_exec(this->database, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::string error = "SQL Error: " + std::to_string(*errorMessage);
        sqlite3_free(errorMessage);
        throw RepositoryException(error);
    }
}

SQLiteRepository::~SQLiteRepository() {
    sqlite3_close(database);
}

TrenchCoat SQLiteRepository::getCoat(std::string name, char size, std::string color) {
    std::vector<TrenchCoat> coats;
    char* errorMessage = nullptr;

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        auto* coats = static_cast<std::vector<TrenchCoat>*>(data);
        try {
            TrenchCoat newCoat = TrenchCoat(argv[0], argv[1][0], argv[2], std::stoi(argv[3]), std::stoi(argv[4]), argv[5]);
            coats->emplace_back(newCoat);
            return SQLITE_OK;
        }
        catch (...) {
            return SQLITE_ERROR;
        }
    };

    std::string sql = "SELECT * FROM trenchcoats "
    "WHERE name = '" + name + "'"
    "AND size = '" + std::string(1, size) + "'"
    "AND color = '" + color + "'";
    if (sqlite3_exec(this->database, sql.c_str(), callback, &coats, &errorMessage) != SQLITE_OK) {
        std::string error = "SQL Error: " + std::to_string(*errorMessage);
        sqlite3_free(errorMessage);
        throw RepositoryException(error);
    }
    if (coats.size() != 1)
        throw RepositoryException("Coat not in repository.\n");
    return coats[0];
}

std::vector<TrenchCoat> SQLiteRepository::getAll() {
    std::vector<TrenchCoat> coats;
    char* errorMessage = nullptr;

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        auto* coats = static_cast<std::vector<TrenchCoat>*>(data);
        try {
            TrenchCoat newCoat = TrenchCoat(argv[0], argv[1][0], argv[2], std::stoi(argv[3]), std::stoi(argv[4]), argv[5]);
            coats->emplace_back(newCoat);
            return SQLITE_OK;
        }
        catch (...) {
            return SQLITE_ERROR;
        }
    };

    const char* sql = "SELECT * FROM trenchcoats";
    if (sqlite3_exec(this->database, sql, callback, &coats, &errorMessage) != SQLITE_OK) {
        std::string error = "SQL Error: " + std::to_string(*errorMessage);
        sqlite3_free(errorMessage);
        throw RepositoryException(error);
    }
    std::sort(coats.begin(), coats.end());
    return coats;
}

bool SQLiteRepository::isInRepo(const TrenchCoat &coat) {
    int count = 0;
    char* errorMessage = nullptr;

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        if (argc > 0 && argv[0]) {
            *static_cast<int*>(data) = 1;
        }
        else
            *static_cast<int*>(data) = 0;
        return 0;
    };

    std::string sql = "SELECT * FROM trenchcoats "
    "WHERE name = '" + coat.getName() + "'"
    "AND size = '" + std::string(1, coat.getSize()) + "'"
    "AND color = '" + coat.getColor() + "'";

    sqlite3_exec(this->database, sql.c_str(), callback, &count, &errorMessage);

    if (errorMessage) {
        sqlite3_free(errorMessage);
        throw RepositoryException("Count failed.\n");
    }
    if (count > 1)
        throw RepositoryException("Duplicate found.\n");


    return count == 1;
}

int SQLiteRepository::size() const {
    int count = 0;
    char* errorMessage = nullptr;

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        if (argc > 0 && argv[0]) {
            *static_cast<int*>(data) = std::stoi(argv[0]);
        }
        return 0;
    };

    sqlite3_exec(this->database, "SELECT COUNT(*) FROM trenchcoats", callback, &count, &errorMessage);

    if (errorMessage) {
        sqlite3_free(errorMessage);
        throw RepositoryException("Count failed.\n");
    }

    return count;
}

void SQLiteRepository::addCoat(const TrenchCoat& coat) {
    std::string sql = "INSERT INTO trenchcoats (name, size, color, price, quantity, link) VALUES ('" +
                     coat.getName() + "', '" +
                     std::string(1, coat.getSize()) + "', '" +
                     coat.getColor() + "', " +
                     std::to_string(coat.getPrice()) + ", " +
                     std::to_string(coat.getQuantity()) + ", '" +
                     coat.getPhoto() + "')";

    executeSQLNoCallback(sql);
}

void SQLiteRepository::removeCoat(const TrenchCoat &coat) {
    if (!this->isInRepo(coat))
        throw RepositoryException("Coat not found.\n");

    std::string sql = "DELETE FROM trenchcoats "
    "WHERE name = '" + coat.getName() + "'"
    "AND size = '" + std::string(1, coat.getSize()) + "'"
    "AND color = '" + coat.getColor() + "'";

    executeSQLNoCallback(sql);
}
void SQLiteRepository::updateCoat(const TrenchCoat &oldCoat, const TrenchCoat &newCoat) {
    this->removeCoat(oldCoat);
    this->addCoat(newCoat);
}

void SQLiteRepository::deleteAll() {
    std::string sql = "DELETE FROM trenchcoats";
    executeSQLNoCallback(sql);
}

