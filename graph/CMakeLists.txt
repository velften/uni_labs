cmake_minimum_required(VERSION 3.29)
project(sem3lab4)

set(CMAKE_CXX_STANDARD 20)

# Включение поддержки CTest для тестирования
enable_testing()

# Добавление поддержки Qt5/Qt6
find_package(Qt5 COMPONENTS Widgets REQUIRED) # Замените на Qt5, если вы используете его
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Основной исполняемый файл
add_executable(sem3lab4
        main.cpp
        src/containers/Stack.h
        src/containers/Queue.h
        src/containers/Graph.h
        src/containers/PriorityQueue.h
        src/containers/DirectedGraph.h
        src/containers/UndirectedGraph.h
        gui/gui.cpp
        gui/gui.h
)

# Подключение Qt библиотек
target_link_libraries(sem3lab4 PRIVATE Qt5::Widgets)

# Добавляем библиотеку Google Test (FetchContent используется для удобства)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Исполняемый файл для тестов
add_executable(tests
        tests/GraphTests.cpp
        src/containers/Stack.h
        src/containers/Queue.h
        src/containers/Graph.h
        src/containers/PriorityQueue.h
        #tests/QueueAndPriorityQueueTests.cpp
)

# Линковка тестов с Google Test
target_link_libraries(tests PRIVATE gtest gtest_main)
add_test(NAME GraphTests COMMAND tests)
