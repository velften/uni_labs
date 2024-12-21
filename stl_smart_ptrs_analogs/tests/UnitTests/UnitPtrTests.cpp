#include "../../srс/ptrs/UnqPtr.h"
#include "../../srс/ptrs/ShrdPtr.h"
//#include "../../src/ptrs/PtrException.h"
#include <gtest/gtest.h>


/// ------------------------default realisation of UnqPtr--------------------------------
TEST(UnqPtrTest, Constructor) {
    int* value = new int(5);
    UnqPtr<int> ptr(value);
    ASSERT_EQ(*ptr, 5);
}

TEST(UnqPtrTest, MoveConstructor) {
    int* value = new int(10);
    UnqPtr<int> ptr1(value);
    UnqPtr<int> ptr2(std::move(ptr1));
    ASSERT_EQ(*ptr2, 10);
    ASSERT_EQ(ptr1.get(), nullptr);
}

TEST(UnqPtrTest, MoveAssignment) {
    int* value1 = new int(15);
    int* value2 = new int(20);
    UnqPtr<int> ptr1(value1);
    UnqPtr<int> ptr2(value2);
    ptr2 = std::move(ptr1);
    ASSERT_EQ(*ptr2, 15);
    ASSERT_EQ(ptr1.get(), nullptr);
}

TEST(UnqPtrTest, MoveSelfAssignment) {
    int* value1 = new int(15);
    UnqPtr<int> ptr1(value1);
    ptr1 = std::move(ptr1);
    ASSERT_EQ(*ptr1, 15);
}

TEST(UnqPtrTest, DereferenceOperator) {
    int* value = new int(25);
    UnqPtr<int> ptr(value);
    ASSERT_EQ(*ptr, 25);
}

TEST(UnqPtrTest, ArrowOperator) {
    struct complexNum {
        int im;
        int re;
    };
    auto* testObj = new complexNum{5, 10};
    UnqPtr<complexNum> ptr(testObj);
    ASSERT_EQ(ptr->im, 5);
    ASSERT_EQ(ptr->re, 10);
}

TEST(UnqPtrTest, GetMethod) {
    int* value = new int(30);
    UnqPtr<int> ptr(value);
    ASSERT_EQ(ptr.get(), value);
}

TEST(UnqPtrTest, ReleaseMethod) {
    int* value = new int(35);
    UnqPtr<int> ptr(value);
    int* released_ptr = ptr.release();
    ASSERT_EQ(released_ptr, value);
    ASSERT_EQ(ptr.get(), nullptr);
    delete released_ptr;
}

TEST(UnqPtrTest, ResetMethod) {
    int* value = new int(40);
    UnqPtr<int> ptr(value);
    int* new_value = new int(50);
    ptr.reset(new_value);
    ASSERT_EQ(*ptr, 50);
    ptr.reset(nullptr);
    ASSERT_EQ(ptr.get(), nullptr);
}

TEST(UnqPtrTest, DereferenceNullptrThrows) {
    UnqPtr<int> ptr(nullptr);
    ASSERT_THROW(*ptr, std::exception);
}



/// ------------------------specific realisation of UnqPtr for arrays--------------------------------

TEST(UnqPtrArrayTest, DefaultConstructor) {
    UnqPtr<int[]> ptr;
    ASSERT_EQ(ptr.get(), nullptr);
}

TEST(UnqPtrArrayTest, Constructor) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr(value);
    ASSERT_EQ(ptr[0], 1);
    ASSERT_EQ(ptr[1], 2);
    ASSERT_EQ(ptr[2], 3);
}

TEST(UnqPtrArrayTest, MoveConstructor) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr1(value);
    UnqPtr<int[]> ptr2(std::move(ptr1));
    ASSERT_EQ(ptr2[0], 1);
    ASSERT_EQ(ptr2[1], 2);
    ASSERT_EQ(ptr2[2], 3);
    ASSERT_EQ(ptr1.get(), nullptr);
}

TEST(UnqPtrArrayTest, MoveSelfAssignment) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr1(value);
    ptr1 = std::move(ptr1);
    ASSERT_EQ(ptr1[0], 1);
}

TEST(UnqPtrArrayTest, MoveAssignment) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr1(value);
    UnqPtr<int[]> ptr2;
    ptr2 = std::move(ptr1);
    ASSERT_EQ(ptr2[0], 1);
    ASSERT_EQ(ptr2[1], 2);
    ASSERT_EQ(ptr2[2], 3);
    ASSERT_EQ(ptr1.get(), nullptr);
}

TEST(UnqPtrArrayTest, OperatorIndex) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr(value);
    ASSERT_EQ(ptr[0], 1);
    ASSERT_EQ(ptr[1], 2);
    ASSERT_EQ(ptr[2], 3);
}

TEST(UnqPtrArrayTest, OperatorIndexOutOfRange) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr(value);
    ASSERT_THROW(ptr[-1], std::exception);
}

TEST(UnqPtrArrayTest, OperatorIndexNullptr) {
    UnqPtr<int[]> ptr;
    ASSERT_THROW(ptr[0], std::exception);
}

TEST(UnqPtrArrayTest, Get) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr(value);
    ASSERT_EQ(ptr.get(), value);
}

TEST(UnqPtrArrayTest, Release) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr(value);
    int* released = ptr.release();
    ASSERT_EQ(released, value);
    ASSERT_EQ(ptr.get(), nullptr);
    delete[] value;
}

TEST(UnqPtrArrayTest, Reset) {
    int* value1 = new int[3]{1, 2, 3};
    int* value2 = new int[3]{4, 5, 6};
    UnqPtr<int[]> ptr(value1);
    ptr.reset(value2);
    ASSERT_EQ(ptr[0], 4);
    ASSERT_EQ(ptr[1], 5);
    ASSERT_EQ(ptr[2], 6);
}


TEST(UnqPtrArrayTest, ResetSamePtr) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr(value);
    ptr.reset(value);
    ASSERT_EQ(ptr[0], 1);
    ASSERT_EQ(ptr[1], 2);
    ASSERT_EQ(ptr[2], 3);
}

TEST(UnqPtrArrayTest, ResetNullptr) {
    int* value = new int[3]{1, 2, 3};
    UnqPtr<int[]> ptr(value);
    ptr.reset();
    ASSERT_EQ(ptr.get(), nullptr);
}

/// ------------------------realisation of ShrdPtr-----------------------------

TEST(ShrdPtrTest, Constructor) {
    ShrdPtr<int> ptr(new int(42));
    EXPECT_EQ(*ptr, 42);
    EXPECT_EQ(ptr.useCount(), 1);
    ShrdPtr<int> ptr2(ptr);
    int* value = new int(42);
    ShrdPtr<int> ptr_scope(value);
    EXPECT_EQ(ptr_scope.useCount(), 1);
    {
        ShrdPtr<int> ptr_scope2 = ptr_scope;
        EXPECT_EQ(ptr.useCount(), 2);
    }
    EXPECT_EQ(ptr.useCount(), 2);
    EXPECT_EQ(ptr2.useCount(), 2);
    EXPECT_EQ(*ptr2, 42);
}


TEST(ShrdPtrTest, MoveConstructor) {
    ShrdPtr<int> ptr(new int(100));
    ShrdPtr<int> movedPtr(std::move(ptr));
    EXPECT_EQ(*movedPtr, 100);
    EXPECT_EQ(movedPtr.useCount(), 1);
    EXPECT_FALSE(ptr);
}

TEST(ShrdPtrTest, DereferenceAndArrowOperator) {
    struct complexNum {
        int re, im;
    };

    ShrdPtr<complexNum> ptr(new complexNum{1, 2});
    EXPECT_EQ(ptr->re, 1);
    EXPECT_EQ(ptr->im, 2);
    EXPECT_EQ((*ptr).re, 1);
    EXPECT_EQ((*ptr).im, 2);
    struct testPtr {
        int* a;
    };
    testPtr* val = nullptr;
    ShrdPtr<testPtr> ptr3(val);
    EXPECT_THROW(ptr3->a, std::exception);
}

TEST(ShrdPtrTest, AssignmentOperator) {
    ShrdPtr<int> ptr1(new int(50));
    ShrdPtr<int> ptr2(new int(75));
    ptr1 = ptr2;
    EXPECT_EQ(ptr1.useCount(), 2);
    EXPECT_EQ(ptr2.useCount(), 2);
    EXPECT_EQ(*ptr1, 75);
    ptr2.reset();
    EXPECT_EQ(ptr1.useCount(), 1);
}

TEST(ShrdPtrTest, SelfAssignmentOperator) {
    ShrdPtr<int> ptr1(new int(50));
    ptr1 = ptr1;
    ShrdPtr<int> ptr2(new int(75));
    ptr1 = ptr2;
    EXPECT_EQ(ptr1.useCount(), 2);
    EXPECT_EQ(ptr2.useCount(), 2);
    EXPECT_EQ(*ptr1, 75);
    ptr2.reset();
    EXPECT_EQ(ptr1.useCount(), 1);
    ShrdPtr<int> ptr3(new int(75));
    ptr2 = ptr3;
}

TEST(ShrdPtrTest, ResetTest) {
    ShrdPtr<int> ptr(new int(20));
    EXPECT_EQ(ptr.useCount(), 1);

    ptr.reset(new int(40));
    EXPECT_EQ(*ptr, 40);
    EXPECT_EQ(ptr.useCount(), 1);

    ptr.reset();
    EXPECT_EQ(ptr.useCount(), 0);
    EXPECT_THROW(*ptr, std::exception);
}


TEST(ShrdPtrTest, SwapTest) {
    ShrdPtr<int> ptr1(new int(30));
    ShrdPtr<int> ptr2(new int(60));

    ptr1.swap(ptr2);
    EXPECT_EQ(*ptr1, 60);
    EXPECT_EQ(*ptr2, 30);
    ShrdPtr<int> nullPtr(nullptr);
    EXPECT_THROW(ptr1.swap(nullPtr), std::exception);
}


TEST(ShrdPtrTest, IsUniqueTest) {
    ShrdPtr<int> ptr(new int(10));
    EXPECT_TRUE(ptr.isUnique());

    ShrdPtr<int> ptr2(ptr);
    EXPECT_FALSE(ptr.isUnique());
    EXPECT_FALSE(ptr2.isUnique());

    ptr2.reset();
    EXPECT_TRUE(ptr.isUnique());
    ShrdPtr<int> ptr3(nullptr);
    EXPECT_THROW(ptr3.isUnique(), std::exception);
}



TEST(ShrdPtrTest, OperatorBoolTest) {
    ShrdPtr<int> ptr(new int(100));
    EXPECT_TRUE(ptr);

    ptr.reset();
    EXPECT_FALSE(ptr);
}


TEST(ShrdPtrTest, GetFunctionTest) {
    ShrdPtr<int> ptr(new int(99));
    EXPECT_EQ(ptr.get(), ptr.operator->());
    ShrdPtr<int> nullPtr(nullptr);
    EXPECT_THROW(nullPtr.get(), std::exception);
}
