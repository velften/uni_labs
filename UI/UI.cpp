#include "UI.h"
#include <gtest/gtest.h>
#include "../srс/collections/DynamicArray.h"
#include "../srс/collections/LinkedList.h"
#include "../srс/ptrs/ShrdPtr.h"
#include "../tests/LoadTesting/Person.h"
#include "../tests/LoadTesting/Timer.h"
#include <vector>
#include <list>



void UI::run() {
    std::string input;
    while (true) {
        MainMenu();
        std::cin >> input;
        if (input == "STOP") {
            std::cout << "Exiting the program." << std::endl;
            break;
        }
        int action;
        try {
            action = std::stoi(input);
        }
        catch (std::invalid_argument&){
            action = 5;
        }
        handleAction(action);
    }
}

void UI::handleAction(int action) {
    switch (action) {
        case 1:
            compareDynamicArrayAndStdVector();
            break;
        case 2:
            compareLinkedListAndStdList();
            break;
        case 3:
            compareUnqPtrAndStdUniquePtr();
            break;
        case 4:
            compareShrdPtrAndStdSharedPtr();
            break;
        default:
            std::cout << "Unknown action. Please try again." << std::endl;
            break;
    }
}

    void UI::MainMenu() {
        std::cout << "---- Main Menu ----" << std::endl;
        std::cout << "1. Compare DynamicArray and std::vector" << std::endl;
        std::cout << "2. Compare LinkedList and std::list" << std::endl;
        std::cout << "3. Compare UnqPtr and std::unique_ptr time creation" << std::endl;
        std::cout << "4. Compare ShrdPtr and std::shared_ptr time creation" << std::endl;
        std::cout << "Enter 'STOP' for finish" << std::endl;
        std::cout << "Choose action: " << std::endl;
    }

void UI::ArrayMenu() {
    std::cout << "---- Array Menu ----" << std::endl;
    std::cout << "1. Append 1000 ints" << std::endl;
    std::cout << "2. Prepend 1000 ints" << std::endl;
    std::cout << "3. Append 1000 Persons" << std::endl;
    std::cout << "4. Append 100000 ints" << std::endl;
    std::cout << "Enter 'return' to get back to Main Menu" << std::endl;
    std::cout << "Choose action: " << std::endl;
}

void UI::ListMenu() {
    std::cout << "---- List Menu ----" << std::endl;
    std::cout << "1. Append 1000 ints" << std::endl;
    std::cout << "2. Prepend 1000 ints" << std::endl;
    std::cout << "3. Append 1000 Persons" << std::endl;
    std::cout << "4. Append 100000 ints" << std::endl;
    std::cout << "Enter 'return' to get back to Main Menu" << std::endl;
    std::cout << "Choose action: " << std::endl;
}


void UI::compareDynamicArrayAndStdVector() {
    std::string input;
    while (true) {
        ArrayMenu();
        std::cin >> input;
        if (input == "return") {
            std::cout << "Going to the main menu." << std::endl;
            break;
        }
        int action;
        try {
            action = std::stoi(input);
        }
        catch (std::invalid_argument&){
            action = 5;
        }
        switch (action) {
            case 1: {
                const size_t size = 1000;
                DynamicArray<int> customArray;
                int item = 1;
                Timer<int, DynamicArray> a(customArray, size);
                std::cout << "DynamicArray creation time: " << a(&DynamicArray<int>::append, item) << " milliseconds"
                          << std::endl;
                std::vector<int> vector;
                Timer<int, std::vector> b(vector, size);
                std::cout << "std::vector creation time: "
                          << b.STLmeasureTime([&vector, item]() { vector.push_back(item); }) << " milliseconds" << std::endl;
            }
                break;
            case 2:
            {
                const size_t size = 1000;
                DynamicArray<int> customArray;
                int item = 1;
                Timer<int, DynamicArray> a(customArray, size);
                std::cout << "DynamicArray creation time: " << a(&DynamicArray<int>::append, item) << " milliseconds"
                          << std::endl;
                std::vector<int> vector;
                Timer<int, std::vector> b(vector, size);
                std::cout << "std::vector creation time: "
                          << b.STLmeasureTime([&vector, item]() { vector.insert(vector.cbegin(), item); }) << " milliseconds"
                          << std::endl;
            }
                break;
            case 3: {
                const size_t size = 1000;
                DynamicArray<Person> customArray;
                Person item ("Ivan","Ivanovich", "Ivanov");
                Timer<Person, DynamicArray> a(customArray, size);
                std::cout << "DynamicArray creation time: " << a(&DynamicArray<Person>::append, item) << " milliseconds"
                          << std::endl;
                std::vector<Person> vector;
                Timer<Person, std::vector> b(vector, size);
                std::cout << "std::vector creation time: "
                          << b.STLmeasureTime([&vector, item]() { vector.push_back(item); }) << " milliseconds" << std::endl;
            }
                break;
            case 4: {
                const size_t size = 100000;
                DynamicArray<int> customArray;
                int item = 1;
                Timer<int, DynamicArray> a(customArray, size);
                std::cout << "DynamicArray creation time: " << a(&DynamicArray<int>::append, item) << " milliseconds"
                          << std::endl;
                std::vector<int> vector;
                Timer<int, std::vector> b(vector, size);
                std::cout << "std::vector creation time: "
                          << b.STLmeasureTime([&vector, item]() { vector.push_back(item); }) << " milliseconds" << std::endl;
            }
                break;
            default:
                std::cout << "Unknown action. Please try again." << std::endl;
                break;
        }
    }
}

void UI::compareLinkedListAndStdList() {
    std::string input;
    while (true) {
        ListMenu();
        std::cin >> input;
        if (input == "return") {
            std::cout << "Going to the main menu." << std::endl;
            break;
        }
        int action;
        try {
            action = std::stoi(input);
        }
        catch (std::invalid_argument&){
            action = 5;
        }
        switch (action) {
            case 1: {
                const size_t size = 1000;
                LinkedList<int> customList;
                int item = 1;
                Timer<int, LinkedList> a(customList, size);
                std::cout << "LinkedList creation time: " << a(&LinkedList<int>::append, item) << " milliseconds"
                          << std::endl;
                std::list<int> list;
                Timer<int, std::list> b(list, size);
                std::cout << "std::list creation time: "
                          << b.STLmeasureTime([&list, item]() { list.push_back(item); }) << " milliseconds" << std::endl;
            }
                break;
            case 2:
            {
                const size_t size = 1000;
                LinkedList<int> customList;
                int item = 1;
                Timer<int, LinkedList> a(customList, size);
                std::cout << "LinkedList creation time: " << a(&LinkedList<int>::prepend, item) << " milliseconds"
                          << std::endl;
                std::list<int> list;
                Timer<int, std::list> b(list, size);
                std::cout << "std::list creation time: "
                          << b.STLmeasureTime([&list, item]() { list.push_front(item); }) << " milliseconds" << std::endl;
            }
                break;
            case 3: {
                const size_t size = 1000;
                LinkedList<Person> customList;
                Person item ("Ivan","Ivanovich", "Ivanov");
                Timer<Person, LinkedList> a(customList, size);
                std::cout << "LinkedList  creation time: " << a(&LinkedList<Person>::append, item) << " milliseconds"
                          << std::endl;
                std::list<Person> list;
                Timer<Person, std::list> b(list, size);
                std::cout << "std::list creation time: "
                          << b.STLmeasureTime([&list, item]() { list.push_back(item); }) << " milliseconds" << std::endl;
            }
                break;
            case 4: {
                const size_t size = 100000;
                LinkedList<int> customList;
                int item = 1;
                Timer<int, LinkedList> a(customList, size);
                std::cout << "LinkedList creation time: " << a(&LinkedList<int>::append, item) << " milliseconds"
                          << std::endl;
                std::list<int> list;
                Timer<int, std::list> b(list, size);
                std::cout << "std::list creation time: "
                          << b.STLmeasureTime([&list, item]() { list.push_back(item); }) << " milliseconds" << std::endl;
            }
                break;
            default:
                std::cout << "Unknown action. Please try again." << std::endl;
                break;
        }
    }
}


void UI::UnqPtrMenu()  {
    std::cout << "---- UnqPtr Menu ----" << std::endl;
    std::cout << "1. Create 1000 ints" << std::endl;
    std::cout << "2. Create 1000 Persons" << std::endl;
    std::cout << "3. Create 100000 ints" << std::endl;
    std::cout << "4. Create your own number of ints" << std::endl;
    std::cout << "5. Create your own number of Persons" << std::endl;
    std::cout << "Enter 'return' to get back to Main Menu" << std::endl;
    std::cout << "Choose action: " << std::endl;
}

void UI::ShrdPtrMenu()  {
    std::cout << "---- ShrdPtr Menu ----" << std::endl;
    std::cout << "1. Create 1000 ints" << std::endl;
    std::cout << "2. Create 1000 Persons" << std::endl;
    std::cout << "3. Create 100000 ints" << std::endl;
    std::cout << "4. Create your own number of ints" << std::endl;
    std::cout << "5. Create your own number of Persons" << std::endl;
    std::cout << "Enter 'return' to get back to Main Menu" << std::endl;
    std::cout << "Choose action: " << std::endl;
}

void UI::compareUnqPtrAndStdUniquePtr() {
    std::string input;
    while (true) {
        UnqPtrMenu();
        std::cin >> input;
        if (input == "return") {
            std::cout << "Going to the main menu." << std::endl;
            break;
        }
        int action;
        try {
            action = std::stoi(input);
        }
        catch (std::invalid_argument&){
            action = 5;
        }
        switch (action) {
            case 1: {
                const size_t size = 1000;
                std::vector<UnqPtr<int>> vector;
                Timer<UnqPtr<int>, std::vector> timer(vector, 1);
                std::cout << "UnqPtr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ UnqPtr<int> a(new int(42));} }) <<
                          " milliseconds"
                          << std::endl;
                std::cout << "std::unique_ptr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ std::unique_ptr<int> a(new int(42));}}) <<
                          " milliseconds"
                          << std::endl;
            }
                break;
            case 2: {
                const size_t size = 1000;
                std::vector<UnqPtr<Person>> vector;
                Timer<UnqPtr<Person>, std::vector> timer(vector, 1);
                std::cout << "UnqPtr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ UnqPtr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));}}) <<
                                                                                                                                        " milliseconds"
                          << std::endl;
                std::cout << "std::unique_ptr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ std::unique_ptr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));}}) <<
                          " milliseconds"
                          << std::endl;
            }
                break;
            case 3: {

                const size_t size = 100000;
                std::vector<UnqPtr<int>> vector;
                std::vector<std::unique_ptr<int>> vector1;
                Timer<UnqPtr<int>, std::vector> timer(vector, 1);
                std::cout << "UnqPtr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ UnqPtr<int> a(new int(42));} }) <<
                          " milliseconds"
                          << std::endl;
                std::cout << "std::unique_ptr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ std::unique_ptr<int> a(new int(42));}}) <<
                          " milliseconds"
                          << std::endl;
            }
                break;
            case 4: {
                std::cout << "Enter the number of creations:" << std::endl;
                int size;
                std::cin >> size;
                if (size <= 0) {
                    std::cout << "Invalid number of creations." << std::endl;
                    break;
                } else {
                    std::vector<UnqPtr<int>> vector;
                    std::vector<std::unique_ptr<int>> vector1;
                    Timer<UnqPtr<int>, std::vector> timer(vector, 1);
                    std::cout << "UnqPtr creation time: "
                              << timer.STLmeasureTime([&size]() {
                                  for (size_t i = 0; i < size; ++i) {
                                      UnqPtr<int> a(new int(42));
                                  }
                              }) <<
                              " milliseconds"
                              << std::endl;
                    std::cout << "std::unique_ptr creation time: "
                              << timer.STLmeasureTime([&size]() {
                                  for (size_t i = 0; i < size; ++i) {
                                      std::unique_ptr<int> a(new int(42));
                                  }
                              }) <<
                              " milliseconds"
                              << std::endl;
                }
            }
                break;
            case 5: {
                std::cout << "Enter the number of creations:" << std::endl;
                int size;
                std::cin >> size;
                if (size <= 0) {
                    std::cout << "Invalid number of creations." << std::endl;
                    break;
                } else {
                    std::vector<UnqPtr<int>> vector;
                    Timer<UnqPtr<int>, std::vector> timer(vector, 1);
                    std::cout << "UnqPtr creation time: "
                              << timer.STLmeasureTime([&size]() { for (size_t i = 0; i < size; ++i){ UnqPtr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));}}) <<
                              " milliseconds"
                              << std::endl;
                    std::cout << "std::unique_ptr creation time: "
                              << timer.STLmeasureTime([&size]() { for (size_t i = 0; i < size; ++i){ std::unique_ptr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));}}) <<
                              " milliseconds"
                              << std::endl;
                }
            }
                break;
            default:
                std::cout << "Unknown action. Please try again." << std::endl;
                break;
        }
    }
}

void UI::compareShrdPtrAndStdSharedPtr() {
    std::string input;
    while (true) {
        ShrdPtrMenu();
        std::cin >> input;
        if (input == "return") {
            std::cout << "Going to the main menu." << std::endl;
            break;
        }
        int action;
        try {
            action = std::stoi(input);
        }
        catch (std::invalid_argument&){
            action = 5;
        }
        switch (action) {
            case 1: {
                const size_t size = 1000;
                std::vector<ShrdPtr<int>> vector;
                Timer<ShrdPtr<int>, std::vector> timer(vector, 1);
                std::cout << "ShrdPtr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ ShrdPtr<int> a(new  int(42));}}) <<
                          " milliseconds"
                          << std::endl;
                std::cout << "std::shared_ptr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ std::shared_ptr<int> a(new  int(42));}}) <<
                          " milliseconds"
                          << std::endl;
            }
                break;
            case 2: {
                const size_t size = 1000;
                std::vector<ShrdPtr<int>> vector;
                Timer<ShrdPtr<int>, std::vector> timer(vector, 1);
                std::cout << "ShrdPtr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ ShrdPtr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));}}) <<
                          " milliseconds"
                          << std::endl;
                std::cout << "std::shared_ptr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ std::shared_ptr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));}}) <<
                          " milliseconds"
                          << std::endl;
            }
                break;
            case 3: {
                const size_t size = 100000;
                std::vector<ShrdPtr<int>> vector;
                Timer<ShrdPtr<int>, std::vector> timer(vector, 1);
                std::cout << "ShrdPtr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ ShrdPtr<int> a(new  int(42));}}) <<
                          " milliseconds"
                          << std::endl;
                std::cout << "std::shared_ptr creation time: "
                          << timer.STLmeasureTime([]() { for (size_t i = 0; i < size; ++i){ std::shared_ptr<int> a(new  int(42));}}) <<
                          " milliseconds"
                          << std::endl;
            }
                break;
            case 4: {
                std::cout << "Enter the number of creations:" << std::endl;
                int size;
                std::cin >> size;
                if (size <= 0) {
                    std::cout << "Invalid number of creations." << std::endl;
                    break;
                } else {
                    std::vector<ShrdPtr<int>> vector;
                    Timer<ShrdPtr<int>, std::vector> timer(vector, 1);
                    std::cout << "ShrdPtr creation time: "
                              << timer.STLmeasureTime([&size]() {
                                  for (size_t i = 0; i < size; ++i) {
                                      ShrdPtr<int> a(new int(42));
                                  }
                              }) <<
                              " milliseconds"
                              << std::endl;
                    std::cout << "std::shared_ptr creation time: "
                              << timer.STLmeasureTime([&size]() {
                                  for (size_t i = 0; i < size; ++i) {
                                      std::shared_ptr<int> a(new int(42));
                                  }
                              }) <<
                              " milliseconds"
                              << std::endl;
                }
            }
                break;
            case 5: {
                std::cout << "Enter the number of creations:" << std::endl;
                int size;
                std::cin >> size;
                if (size <= 0) {
                    std::cout << "Invalid number of creations." << std::endl;
                    break;
                } else {
                    std::vector<UnqPtr<int>> vector;
                    Timer<UnqPtr<int>, std::vector> timer(vector, 1);
                    std::cout << "ShrdPtr creation time: "
                              << timer.STLmeasureTime([&size]() { for (size_t i = 0; i < size; ++i){ ShrdPtr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));}}) <<
                              " milliseconds"
                              << std::endl;
                    std::cout << "std::shared_ptr creation time: "
                              << timer.STLmeasureTime([&size]() { for (size_t i = 0; i < size; ++i){ std::shared_ptr<Person> a(new Person("Ivan", "Ivanovich", "Ivanov"));
                              }}) <<
                              " milliseconds"
                              << std::endl;
                }
            }
            default:
                std::cout << "Unknown action. Please try again." << std::endl;
                break;
        }
    }
}

