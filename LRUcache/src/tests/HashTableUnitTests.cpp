#include <gtest/gtest.h>
#include "../containers/HashTable.h"
#include <string>
#include <stdexcept>

TEST(HashTableTest, AddAndGet) {
    HashTable<int, std::string> table;


    table.add(1, "One");
    table.add(2, "Two");

    EXPECT_EQ(table.get(1), "One");
    EXPECT_EQ(table.getCount(), 2);
    EXPECT_EQ(table.getCapacity(), 11);

    HashTable<int, std::string> copyOfTable = table;
    EXPECT_EQ(copyOfTable.get(1), "One");
    EXPECT_EQ(copyOfTable.getCount(), 2);
    EXPECT_EQ(copyOfTable.getCapacity(), 11);

    HashTable<int, std::string> movedTable(std::move(table));
    EXPECT_EQ(movedTable.get(1), "One");
    EXPECT_EQ(movedTable.getCount(), 2);
    EXPECT_EQ(movedTable.getCapacity(), 11);


}

TEST(HashTableTest, ContainsKey) {
    HashTable<int, std::string> table;

    table.add(1, "One");
    table.add(2, "Two");

    EXPECT_TRUE(table.containsKey(1));
    EXPECT_TRUE(table.containsKey(2));
    EXPECT_FALSE(table.containsKey(3));
}

TEST(HashTableTest, Remove) {
    HashTable<int, std::string> table;

    table.add(1, "One");
    table.add(2, "Two");

    table.remove(1);
    EXPECT_FALSE(table.containsKey(1));

    EXPECT_THROW(table.remove(3), std::out_of_range);
}

TEST(HashTableTest, IndexOperator) {
    HashTable<int, std::string> table;

    table.add(1, "One");
    table.add(2, "Two");

    EXPECT_EQ(table[1], "One");
    EXPECT_EQ(table[2], "Two");

    EXPECT_THROW(table[3], std::out_of_range);
}

TEST(HashTableTest, Iterator) {
    HashTable<int, std::string> table;

    table.add(1, "One");
    table.add(2, "Two");

    std::vector<std::string> values;
    for (auto i = table.beginC(); i != table.endC(); i++) {
        values.push_back(*i);
    }

    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], "One");
    EXPECT_EQ(values[1], "Two");
}

TEST(HashTableTest, Resize) {
    HashTable<int, std::string> table;

    for (int i = 0; i < 100000; ++i) {
        table.add(i, "Test" + std::to_string(i));
    }


    for (int i = 0; i < 100000; ++i) {
        EXPECT_TRUE(table.containsKey(i));
    }
}

TEST(HashTableTest, Rebuild) {
    HashTable<int, std::string> table;


    for (int i = 0; i < 100000; ++i) {
        table.add(i, "Test" + std::to_string(i));
    }


    for (int i = 0; i < 100000; ++i) {
        EXPECT_EQ(table.get(i), "Test" + std::to_string(i));
    }
}
