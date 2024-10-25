cmake_minimum_required(VERSION 3.11)
project(sem3lab1)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")


enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)


add_executable(sem3lab1
        main.cpp
        UI/UI.cpp
        srс/ptrs/PtrException.h
)

add_executable(UnitTests
        tests/UnitTests/UnitPtrTests.cpp
        tests/UnitTests/UnitLinkedListTests.cpp
        tests/UnitTests/UnitDArrayTests.cpp
)

target_link_libraries(UnitTests gtest gtest_main)

add_test(NAME MyUnitTests COMMAND UnitTests)
