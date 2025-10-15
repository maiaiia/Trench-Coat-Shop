#include "tests.h"

void testInvalidCoat(TrenchCoat& invalidCoat) {
    try {
        TrenchCoatValidator::validateCoat(invalidCoat);
        assert(false);
    }
    catch (ValidatorException&) {
        assert(true);
    }
}
void testCoatValidation() {
    TrenchCoatValidator::validateSize('S');
    TrenchCoatValidator::validateSize('M');
    TrenchCoatValidator::validateSize('L');
    try {
        TrenchCoatValidator::validateSize('X');
        assert(false);
    }
    catch (ValidatorException&) {
        assert(true);
    }

    TrenchCoat invalidCoat{"coat", 'S',"purple",120,50,"https://link"};
    invalidCoat.setName("");
    testInvalidCoat(invalidCoat);
    invalidCoat.setName("Invalid' n@m3");
    testInvalidCoat(invalidCoat);
    invalidCoat.setName("Coat");
    invalidCoat.setSize('W');
    testInvalidCoat(invalidCoat);
    invalidCoat.setSize('L');
    invalidCoat.setColor("");
    testInvalidCoat(invalidCoat);
    invalidCoat.setColor("purple");
    invalidCoat.setPrice(-100);
    testInvalidCoat(invalidCoat);
    invalidCoat.setPrice(120);
    invalidCoat.setQuantity(-100);
    testInvalidCoat(invalidCoat);
    invalidCoat.setQuantity(50);
    invalidCoat.setLink("");
    testInvalidCoat(invalidCoat);
    invalidCoat.setLink("https://link");
    TrenchCoatValidator::validateCoat(invalidCoat);
}

const std::string TEST_FILE = "../tests/test_file.txt";

void writeDataToTextFile() {
    std::ofstream fout(TEST_FILE);
    fout << "Urban Safari Coat,S,Green,175,25,https://img.co/urban-safari\n";
    fout << "Urban Safari Coat,M,Green,175,30,https://img.co/urban-safari\n";
    fout << "Urban Safari Coat,L,Green,175,15,https://img.co/urban-safari\nClassic Noir Trench,S,Black,120,18,https://img.co/classic-noir\nClassic Noir Trench,M,Black,120,50,https://img.co/classic-noir\nClassic Noir Trench,L,Black,120,35,https://img.co/classic-noir\nAdventurer Coat,S,Beige,130,20,https://img.co/adventurer\nAdventurer Coat,M,Beige,130,40,https://img.co/adventurer\nAdventurer Coat,L,Beige,130,20,https://img.co/adventurer\nAdventurer Coat,S,Olive,130,30,https://img.co/adventurer\nAdventurer Coat,M,Olive,100,31,https://img.co/adventurer\nWrap Coat,S,Navy,84,65,https://img.co/wrap-coat\nWrap Coat,M,Navy,84,60,https://img.co/wrap-coat\nChesterfield Coat,S,Burgundy,200,15,https://img.co/chesterfield\nChesterfield Coat,M,Burgundy,200,6,https://img.co/chesterfield\nChesterfield Coat,L,Burgundy,200,6,https://img.co/chesterfield\n";
    fout.close();
}
void clearTextFile() {
    std::ofstream fout(TEST_FILE); fout.close();
}

void testRepository(TrenchCoatRepository& repo) {
    TrenchCoat testCoat = TrenchCoat("Test Coat", 'S', "Color", 100, 120, "https://img.co/link");
    assert(repo.getAll().size()==0);
    assert(repo.size() == repo.getAll().size());
    assert(!repo.isInRepo(testCoat));
    repo.addCoat(testCoat);
    assert(repo.isInRepo(testCoat));
    testCoat.setSize('M');
    repo.addCoat(testCoat);
    assert(repo.isInRepo(testCoat));
    testCoat.setSize('L');
    assert(!repo.isInRepo(testCoat));
    repo.addCoat(testCoat);
    assert(repo.isInRepo(testCoat));
    assert(repo.size()==3);
    TrenchCoat newCoat = repo.getCoat(testCoat.getName(), testCoat.getSize(), testCoat.getColor());
    assert(newCoat == testCoat);
    newCoat.setPrice(1000);
    repo.updateCoat(testCoat, newCoat);
    assert(repo.getCoat(testCoat.getName(), testCoat.getSize(), testCoat.getColor()).getPrice() == 1000);
    assert(repo.getAll().size()==3);
    try {
        repo.removeCoat(testCoat);
        assert(true);
    }
    catch (...) {
        assert(false);
    }
    try {
        repo.removeCoat(testCoat);
        assert(false);
    }
    catch (RepositoryException&) {
        assert(true);
    }
    assert(!repo.isInRepo(testCoat));
    try {
        repo.getCoat(testCoat.getName(),testCoat.getSize(), testCoat.getColor());
        assert(false);
    }
    catch (RepositoryException&) {
        assert(true);
    }

}
void testAllRepos() {
    {
        clearTextFile();
        TextFileRepository textFileRepository = TextFileRepository(TEST_FILE);
        testRepository(textFileRepository);


        TextFileRepository copyRepository(textFileRepository);
        assert(copyRepository.size() == textFileRepository.size());
    }

    {
        SQLiteRepository sqliteRepository = SQLiteRepository("../tests/test_file.db");
        sqliteRepository.deleteAll();
        testRepository(sqliteRepository);
    }
}



void testAdminService() {
    std::ofstream fout(TEST_FILE);
    fout << "Urban Safari Coat,S,Green,175,25,https://img.co/urban-safari\n";
    fout << "Urban Safari Coat,M,Green,175,30,https://img.co/urban-safari\n";
    fout << "Urban Safari Coat,L,Green,175,15,https://img.co/urban-safari\nClassic Noir Trench,S,Black,120,18,https://img.co/classic-noir\nClassic Noir Trench,M,Black,120,50,https://img.co/classic-noir\nClassic Noir Trench,L,Black,120,35,https://img.co/classic-noir\nAdventurer Coat,S,Beige,130,20,https://img.co/adventurer\nAdventurer Coat,M,Beige,130,40,https://img.co/adventurer\nAdventurer Coat,L,Beige,130,20,https://img.co/adventurer\nAdventurer Coat,S,Olive,130,30,https://img.co/adventurer\nAdventurer Coat,M,Olive,100,31,https://img.co/adventurer\nWrap Coat,S,Navy,84,65,https://img.co/wrap-coat\nWrap Coat,M,Navy,84,60,https://img.co/wrap-coat\nChesterfield Coat,S,Burgundy,200,15,https://img.co/chesterfield\nChesterfield Coat,M,Burgundy,200,6,https://img.co/chesterfield\nChesterfield Coat,L,Burgundy,200,6,https://img.co/chesterfield\n";
    fout.close();

    TextFileRepository repository = TextFileRepository(TEST_FILE);
    AdminService adminService(repository);
    assert(adminService.size() == 16);
    std::vector<TrenchCoat> allCoats = adminService.getAllCoats();
    assert(adminService.size() == allCoats.size());
    /*
    assert(adminService.getCoatPosition(TrenchCoat("Adventurer Coat",'S',"Beige",130,20,"https://img.co/adventurer"))==0);
    assert(adminService.getCoat(14)==TrenchCoat("Wrap Coat",'S',"Navy",84,65,"https://img.co/wrap-coat"));
    */
    adminService.addCoat(TrenchCoat("Test Coat", 'L',"Purple", 100,20,"https://test_photo.com"));
    assert(adminService.size() == 17);
    TrenchCoat testCoat = TrenchCoat("Test Coat", 'L',"Purple");
    try {
        adminService.addCoat(testCoat);
        assert(false);
    }
    catch (...) {
        assert(true);
    }
    try {
        adminService.removeCoat(testCoat);
        assert(false);
    }
    catch (...) {
        assert(true);
    }


    assert(adminService.size() == 17);
    std::string attribute = "quantity";
    adminService.updateCoat(testCoat,0,attribute);
    //assert(adminService.getCoat(11).getQuantity()==0);
    attribute = "price";
    adminService.updateCoat(testCoat,1000,attribute);
    //assert(adminService.getCoat(11).getPrice()==1000);

    try {
        TrenchCoat coat = TrenchCoat("NonExistentCoat",'L',"Aqua");
        adminService.updateCoat(coat,12,attribute);
        assert(false);
    }
    catch (...) {
        assert(true);
    }
    adminService.updateLink(testCoat, "https://img.co/new-link");
    try {
        TrenchCoat coat = TrenchCoat("NonExistentCoat",'L',"Aqua");
        adminService.updateLink(coat,"newlink");
        assert(false);
    }
    catch (...) {
        assert(true);
    }
    try {
        attribute="wrongattribute";
        adminService.updateCoat(testCoat,18,attribute);
        assert(false);
    }
    catch (...) {
        assert(true);
    }
    adminService.removeCoat(testCoat);
    try {
        adminService.removeCoat(testCoat);
        assert(false);
    }
    catch (ServiceException&) {
        assert(true);
    }
}
void testUserService() {

    clearTextFile();
    {
        CSVBasket basket = CSVBasket{"../tests/test_basket.csv"};
        TextFileRepository repository = TextFileRepository(TEST_FILE);
        UserService service(repository, basket);
        try {
            service.getCoatsBySize('S');
            assert(false);
        }
        catch (ServiceException&) {
            assert(true);
        }
    }

    writeDataToTextFile();
    HTMLBasket basket = HTMLBasket("../tests/test_basket.html");
    TextFileRepository repository = TextFileRepository(TEST_FILE);
    UserService service{repository, basket};
    std::vector<TrenchCoat> smallCoats = service.getCoatsBySize('S');
    assert(smallCoats.size()==6);
    service.saveBasket();
}

void testAll() {
    testCoatValidation();
    testAllRepos();
    testAdminService();
    testUserService();
}
