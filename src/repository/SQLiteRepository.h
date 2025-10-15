#pragma once
#include "TrenchCoatRepository.h"
#include <../sql/sqlite3.h>

class SQLiteRepository: public TrenchCoatRepository{
    sqlite3* database;
    std::string filename;
public:
    explicit  SQLiteRepository(std::string filename);
    ~SQLiteRepository() override;
    bool isInRepo(const TrenchCoat& coat) override;
    int size() const override;
    std::vector<TrenchCoat> getAll() override;

    TrenchCoat getCoat(std::string name, char size, std::string color) override;
    void addCoat (const TrenchCoat &coat) override;
    void removeCoat (const TrenchCoat &coat) override;
    void updateCoat(const TrenchCoat &oldCoat, const TrenchCoat &newCoat) override;

    void deleteAll(); //used for tests only
private:
    void initializeDatabase();
    void executeSQLNoCallback(const std::string sql) const;
};