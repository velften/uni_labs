cmake_minimum_required(VERSION 3.28)
project(sem3lab3)

# Установить стандарт языка C++
set(CMAKE_CXX_STANDARD 20)

# Включить поддержку автоматической генерации MOC файлов для Qt
set(CMAKE_AUTOMOC ON)

# Установить опции компиляции для покрытия
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")

# Включить поддержку тестирования
enable_testing()

# Найти Qt5
find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

# Скачать и подключить Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# Автоматически скачать и подготовить Google Test
FetchContent_MakeAvailable(googletest)

# Главный исполняемый файл
add_executable(sem3lab3
        main.cpp
        UI/MainWindow.h
        UI/MainWindow.cpp
        src/containers/Hashing.h
)

# Подключить библиотеки Qt к таргету `sem3lab3`
target_link_libraries(sem3lab3 PRIVATE Qt5::Widgets Qt5::Charts)

# Тесты
add_executable(UnitTests
        src/tests/UnitDoubleLinkedListTests.cpp
        src/tests/HashTableUnitTests.cpp
        src/tests/LRUcacheUnitTests.cpp
)

# Подключить Google Test к таргету UnitTests
target_link_libraries(UnitTests PRIVATE gtest_main gtest)

# Указать include-директории для тестов
target_include_directories(UnitTests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Установить UnitTests как тестовый таргет
add_test(NAME UnitTests COMMAND UnitTests)
