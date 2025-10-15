cmake_minimum_required(VERSION 3.10)
project(Trench-Coat-Shop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH /opt/homebrew/Cellar/qt@5/5.15.16_2)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(src/domain src/repository src/service src/ui tests src/utils src/utils/sql src/utils/statistics src/ui/models)

add_executable(a10
        main.cpp
        src/domain/TrenchCoat.cpp
        src/repository/RepositoryException.cpp
        src/repository/TextFileRepository.cpp
        src/service/ServiceException.cpp
        src/service/AdminService.cpp
        src/service/UserService.cpp
        src/ui/CUI.cpp
        tests/tests.cpp
        src/domain/TrenchCoatValidator.cpp
        src/repository/ShoppingBasket.cpp
        src/repository/FileBasket.cpp
        src/repository/SQLiteRepository.cpp
        src/repository/SQLiteRepository.h
        src/utils/sql/sqlite3.c
        src/ui/UI.cpp
        src/ui/UI.h
        src/ui/GUI.cpp
        src/ui/GUI.h
        src/utils/statistics/StatisticsGenerator.cpp
        src/utils/statistics/StatisticsGenerator.h
        src/service/UndoService.cpp
        src/service/UndoService.h
        src/utils/action/Action.cpp
        src/utils/action/Action.h
        src/ui/models/BasketModel.cpp
        src/ui/models/BasketModel.h
)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

target_link_libraries(a10
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
)


