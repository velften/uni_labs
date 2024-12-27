#include <gtest/gtest.h>
#include "../containers/DoubleLinkedList.h"


TEST(DoubleLinkedListTest, ConstructorDestructor)  {
    DoubleLinkedList<int> list;
    EXPECT_EQ(list.getLength(), 0);
    DoubleLinkedList<int>::Node(8);
}

TEST(DoubleLinkedListTest, Append) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    EXPECT_EQ(list.getLength(), 3);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);
}

TEST(DoubleLinkedListTest, Prepend) {
    DoubleLinkedList<int> list;
    list.prepend(1);
    list.prepend(2);
    list.prepend(3);

    EXPECT_EQ(list.getLength(), 3);
    EXPECT_EQ(list.get(0), 3);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 1);

}

TEST(DoubleLinkedListTest, InsertAt) {
    DoubleLinkedList<int> list;
    list.insertAt(1, 0);
    list.append(3);

    list.insertAt(2, 1);
    list.insertAt(3, 2);

    EXPECT_EQ(list.getLength(), 4);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);

    EXPECT_THROW(list.insertAt(4, 5), std::exception);
}

TEST(DoubleLinkedListTest, Get) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);

    EXPECT_THROW(list.get(3), std::exception);
}

TEST(DoubleLinkedListTest, GetSubsequence) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);

    DoubleLinkedList<int>* subList = list.getSubsequence(1, 2);
    EXPECT_THROW(DoubleLinkedList<int>* subList1 = list.getSubsequence(-11, 2), std::exception);

    EXPECT_EQ(subList->getLength(), 2);
    EXPECT_EQ((*subList)[0], 2);
    EXPECT_EQ((*subList)[1], 3);

    delete subList;
}

TEST(DoubleLinkedListTest, Concat) {
    DoubleLinkedList<int> list1;
    list1.append(1);
    list1.append(2);

    DoubleLinkedList<int> list2;
    list1.concat(list2);
    EXPECT_EQ(list1.getLength(), 2);
    list2.append(3);
    list2.append(4);

    list1.concat(list2);
    DoubleLinkedList<int> list3;
    list3.concat(list1);
    EXPECT_EQ(list3.getLength(), 4);
    EXPECT_EQ(list3.getFirst(), 1);
    EXPECT_EQ(list1.getLength(), 4);
    EXPECT_EQ(list1.getFirst(), 1);
    EXPECT_EQ(list1.get(1), 2);
    EXPECT_EQ(list1.get(2), 3);
    EXPECT_EQ(list1.getLast(), 4);
}


TEST(DoubleLinkedListTest, Find) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    EXPECT_EQ(list.find(2), 1);
    EXPECT_EQ(list.find(4), -1);
}

TEST(DoubleLinkedListTest, Map) {
    auto square = [](int x) { return x * x; };
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    list.map(square);

    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 4);
    EXPECT_EQ(list.get(2), 9);
}

TEST(DoubleLinkedListTest, Reduce) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    int sum = list.reduce([](int a, int b) { return a + b; }, 0);

    EXPECT_EQ(sum, 6);
}

TEST(DoubleLinkedListTest, Where) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);

    DoubleLinkedList<int>* filteredList = list.where([](int x) { return x % 2 == 0; });

    EXPECT_EQ(filteredList->getLength(), 2);
    EXPECT_EQ((*filteredList)[0], 2);
    EXPECT_EQ((*filteredList)[1], 4);

    delete filteredList;
}

TEST(DoubleLinkedListTest, EqualityOperator) {
    DoubleLinkedList<int> list1;
    list1.append(1);
    list1.append(2);

    DoubleLinkedList<int> list2;
    list2.append(1);
    list2.append(2);

    DoubleLinkedList<int> list3;
    list3.append(3);
    EXPECT_FALSE(list1 == list3);
    list3.append(4);

    EXPECT_TRUE(list1 == list2);
    EXPECT_FALSE(list1 == list3);
}

TEST(DoubleLinkedListTest, InequalityOperator) {
    DoubleLinkedList<int> list1;
    list1.append(1);
    list1.append(2);

    DoubleLinkedList<int> list2;
    list2.append(1);
    list2.append(2);

    DoubleLinkedList<int> list3;
    list3.append(3);
    list3.append(4);

    EXPECT_FALSE(list1 != list2);
    EXPECT_TRUE(list1 != list3);
    list2.append(2);
    list2.append(2);
    list2.append(2);
    EXPECT_TRUE(list1 != list2);
}

TEST(DoubleLinkedListTest, AccessOperator) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);

    EXPECT_THROW(list[3], std::exception);
}

TEST(DoubleLinkedListTest, GetNode){
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    DoubleLinkedList<int> list2;
    list2.append(1);
    list2.append(2);
    list.concat(list2);
    EXPECT_THROW(list.getNode(-1), std::exception);
    EXPECT_EQ(*(list.getNode(2)->data), 1);
}
TEST(LinkedListTest, Delete) {
    DoubleLinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
   list.del(0);
    list.del(1);

    EXPECT_EQ(list.getLength(), 1);
    EXPECT_EQ(*((list.getHead())->data), 2);
    list.append(3);
    EXPECT_EQ(list.get(1), 3);

    EXPECT_THROW(list.del(2), std::exception);
}