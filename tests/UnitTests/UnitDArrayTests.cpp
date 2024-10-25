#include <gtest/gtest.h>
#include "../../srс/collections/DynamicArray.h"

TEST(DynamicArrayTest, DefaultConstructor) {
    DynamicArray<int> array;
    EXPECT_EQ(array.getLength(), 0);
}

TEST(DynamicArrayTest, ParameterizedConstructorValid) {
    int* values = new int[3]{1, 2, 3};
    DynamicArray<int> array(values, 3, 3);
    EXPECT_EQ(array.getLength(), 3);
    EXPECT_EQ(array.get(0), 1);
    EXPECT_EQ(array.get(1), 2);
    EXPECT_EQ(array.get(2), 3);
}


TEST(DynamicArrayTest, ParameterizedConstructorInvalidCapacity) {
    EXPECT_THROW({
                     int* ptr = nullptr;
                     DynamicArray<int> array(ptr, -1, 0);
                 }, const char*);
}


TEST(DynamicArrayTest, ParameterizedConstructorInvalidLength) {
    EXPECT_THROW({
                     int* ptr = nullptr;
                     DynamicArray<int> array(ptr, 1, -1);
                 }, const char*);
}


TEST(DynamicArrayTest, ParameterizedConstructorNullPointer) {
    EXPECT_THROW({
                     int* ptr = nullptr;
                     DynamicArray<int> array(ptr, 0, 0);
                 }, const char*);
}


TEST(DynamicArrayTest, CopyConstructor) {
    int* values = new int[3]{1, 2, 3};
    DynamicArray<int> array1(values, 3, 3);
    DynamicArray<int> array2 = array1;
    EXPECT_EQ(array2.getLength(), 3);
    EXPECT_EQ(array2.get(0), 1);
    EXPECT_EQ(array2.get(1), 2);
    EXPECT_EQ(array2.get(2), 3);
    array2.append(1);
}


TEST(DynamicArrayTest, MoveConstructor) {
    int* values = new int[3]{1, 2, 3};
    DynamicArray<int> array1(values, 3, 3);
    DynamicArray<int> array2 = std::move(array1);
    EXPECT_EQ(array2.getLength(), 3);
    EXPECT_EQ(array2.get(0), 1);
    EXPECT_EQ(array2.get(1), 2);
    EXPECT_EQ(array2.get(2), 3);
    EXPECT_EQ(array1.getLength(), 0);
}


TEST(DynamicArrayTest, CopyAssignmentOperator) {
    int* values1 = new int[3]{1, 2, 3};
    DynamicArray<int> array1(values1, 3, 3);

    int* values2 = new int[2]{4,5};
    DynamicArray<int> array2(values2, 2, 2);

    array2 = array1;
    DynamicArray<int> arr3;
    EXPECT_THROW(arr3.get(2), const char*);
    EXPECT_EQ(array2.getLength(), 3);
    EXPECT_EQ(array2.get(0), 1);
    EXPECT_EQ(array2.get(1), 2);
    EXPECT_EQ(array2.get(2), 3);
}


TEST(DynamicArrayTest, MoveAssignmentOperator) {
    int* values1 = new int[3]{1, 2, 3};
    DynamicArray<int> array1(values1, 3, 3);
    int* values2 = new int[2]{4, 5};
    DynamicArray<int> array2(values2, 2, 2);

    array2 = std::move(array1);
    EXPECT_EQ(array2.getLength(), 3);
    EXPECT_EQ(array2.get(0), 1);
    EXPECT_EQ(array2.get(1), 2);
    EXPECT_EQ(array2.get(2), 3);
    EXPECT_EQ(array1.getLength(), 0);
}


TEST(DynamicArrayTest, GetMethod) {
    int* values = new int[3]{1, 2, 3};
    DynamicArray<int> array(values, 3, 3);
    EXPECT_EQ(array.get(0), 1);
    EXPECT_EQ(array.get(1), 2);
    EXPECT_EQ(array.get(2), 3);
    EXPECT_THROW(array.get(3), const char*);
}


TEST(DynamicArrayTest, GetFirstAndLast) {
    int* values = new int[3]{1, 2, 3};
    DynamicArray<int> array(values, 3, 3);
    EXPECT_EQ(array.getFirst(), 1);
    EXPECT_EQ(array.getLast(), 3);
}


TEST(DynamicArrayTest, AppendMethod) {
    DynamicArray<int> array;
    array.append(1);
    array.append(2);
    array.append(3);
    EXPECT_EQ(array.getLength(), 3);
    EXPECT_EQ(array.get(0), 1);
    EXPECT_EQ(array.get(1), 2);
    EXPECT_EQ(array.get(2), 3);
}


TEST(DynamicArrayTest, PrependMethod) {
    DynamicArray<int> array;
    array.append(2);
    array.append(3);
    array.prepend(1);
    int* vals = new int[2]{1, 2};
    DynamicArray<int> array2 = DynamicArray<int>(vals, 2, 2);
    array2.prepend(1);
    EXPECT_EQ(array.getLength(), 3);
    EXPECT_EQ(array2.getLength(), 3);
    EXPECT_EQ(array.get(0), 1);
    EXPECT_EQ(array2.get(0), 1);
    EXPECT_EQ(array.get(1), 2);
    EXPECT_EQ(array.get(2), 3);

}

TEST(DynamicArrayTest, InsertAtMethod) {
    DynamicArray<int> array;
    array.append(1);
    array.append(3);
    array.insertAt(2, 1);

    EXPECT_EQ(array.getLength(), 3);
    EXPECT_EQ(array.get(0), 1);
    EXPECT_EQ(array.get(1), 2);
    EXPECT_EQ(array.get(2), 3);
    int* values = new int[4]{1, 2, 3, 4};
    DynamicArray<int> array1(values, 4, 4);
    EXPECT_THROW(array1.insertAt(2, -1), const char*);
    array1.insertAt(2, 1);
    EXPECT_EQ(array1.get(1), 2);

}

TEST(DynamicArrayTest, GetSubsequenceMethod) {
    int* values = new int[4]{1, 2, 3, 4};
    DynamicArray<int> array(values, 4, 4);

    DynamicArray<int>* subArray (array.getSubsequence(1, 2));
    EXPECT_THROW(array.getSubsequence(-1, 2), const char*);
    EXPECT_EQ(subArray->getLength(), 2);
    EXPECT_EQ(subArray->get(0), 2);
    EXPECT_EQ(subArray->get(1), 3);
    delete subArray;
}


TEST(DynamicArrayTest, ResizeOnAppend) {
    DynamicArray<int> array;
    for (int i = 0; i < 10; ++i) {
        array.append(i);
    }
    EXPECT_EQ(array.getLength(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(array.get(i), i);
    }
}

TEST(DynamicArrayTest, Concat) {
    DynamicArray<int> arr1;
    DynamicArray<int> arr_h;
    arr1.concat(arr_h);
    arr1.append(1);
    arr1.append(2);
    arr1.append(3);

    DynamicArray<int> arr2;
    arr2.append(4);
    arr2.append(5);
    arr2.append(6);

    arr1.concat(arr2);

    EXPECT_EQ(arr1.getLength(), 6);
    EXPECT_EQ(arr1[0], 1);
    EXPECT_EQ(arr1[1], 2);
    EXPECT_EQ(arr1[2], 3);
    EXPECT_EQ(arr1[3], 4);
    EXPECT_EQ(arr1[4], 5);
    EXPECT_EQ(arr1[5], 6);
}

TEST(DynamicArrayTest, Find) {
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.append(3);

    EXPECT_EQ(arr.find(2), 1);
    EXPECT_EQ(arr.find(4), -1);
}

TEST(DynamicArrayTest, Map) {
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.append(3);

    auto squared = [](int x) { return x * x; };
    arr.map(squared);

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 4);
    EXPECT_EQ(arr[2], 9);
}

TEST(DynamicArrayTest, Del) {
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.append(3);
    arr.append(4);
    arr.append(5);


    arr.del(2);
    EXPECT_EQ(arr.getLength(), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 4);
    EXPECT_EQ(arr[3], 5);

    arr.del(0);
    EXPECT_EQ(arr.getLength(), 3);
    EXPECT_EQ(arr[0], 2);
    EXPECT_EQ(arr[1], 4);
    EXPECT_EQ(arr[2], 5);

    arr.del(2);
    EXPECT_EQ(arr.getLength(), 2);
    EXPECT_EQ(arr[0], 2);
    EXPECT_EQ(arr[1], 4);

    EXPECT_THROW(arr.del(3), const char*);
}

TEST(DynamicArrayTest, DelEdgeCases) {
    DynamicArray<int> arr;
    arr.append(1);
    arr.del(0);
    EXPECT_EQ(arr.getLength(), 0);
    DynamicArray<int> emptyArr;
    EXPECT_THROW(emptyArr.del(0), const char*);
}

TEST(DynamicArrayTest, Reduce) {
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.append(3);

    auto sum = [](int x, int y) { return x + y; };
    EXPECT_EQ(arr.reduce(sum, 0), 6);
}

TEST(DynamicArrayTest, Where) {
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.append(3);
    arr.append(4);
    arr.append(5);

    auto isEven = [](int x) { return x % 2 == 0; };
    auto result = arr.where(isEven);

    EXPECT_EQ(result->getLength(), 2);
    EXPECT_EQ((*result)[0], 2);
    EXPECT_EQ((*result)[1], 4);
    delete result;
}

TEST(DynamicArrayTest, OperatorIndex) {
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.append(3);

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(DynamicArrayTest, OperatorEquals) {
    DynamicArray<int> arr1;
    arr1.append(1);
    arr1.append(2);
    arr1.append(3);

    DynamicArray<int> arr2;
    arr2.append(1);
    arr2.append(2);
    arr2.append(3);

    EXPECT_TRUE(arr1 == arr2);
    arr2.append(3);
    EXPECT_FALSE(arr1 == arr2);
}

TEST(DynamicArrayTest, OperatorNotEquals) {
    DynamicArray<int> arr1;
    arr1.append(1);
    arr1.append(2);
    arr1.append(3);

    DynamicArray<int> arr2;
    arr2.append(1);
    arr2.append(2);
    arr2.append(4);

    EXPECT_TRUE(arr1 != arr2);
}
