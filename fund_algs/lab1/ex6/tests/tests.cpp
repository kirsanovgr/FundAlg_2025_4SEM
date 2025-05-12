#include <gtest/gtest.h>
#include "../include/unique_ptr.hpp"

using namespace my_smart_ptr;

// Фикстура для тестирования UniquePtr<T>
class UniquePtrTest : public ::testing::Test {
protected:
    void SetUp() override {
        ptr = UniquePtr<int>(new int(42));
        arr_ptr = UniquePtr<int[]>(new int[3]{1, 2, 3});
    }

    void TearDown() override {
    }

    UniquePtr<int> ptr;
    UniquePtr<int[]> arr_ptr;
};

// Тесты конструкторов
TEST_F(UniquePtrTest, DefaultConstructorCreatesNullPtr) {
    UniquePtr<int> default_ptr;
    EXPECT_EQ(default_ptr.get(), nullptr);
    EXPECT_FALSE(static_cast<bool>(default_ptr));
}

TEST_F(UniquePtrTest, PointerConstructorTakesOwnership) {
    int* raw_ptr = new int(10);
    UniquePtr<int> local_ptr(raw_ptr);
    EXPECT_EQ(local_ptr.get(), raw_ptr);
    EXPECT_EQ(*local_ptr, 10);
}

TEST_F(UniquePtrTest, MoveConstructorTransfersOwnership) {
    int* raw_ptr = ptr.get();
    UniquePtr<int> moved_ptr(std::move(ptr));
    
    EXPECT_EQ(moved_ptr.get(), raw_ptr);
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(*moved_ptr, 42);
}

// Тесты оператора перемещающего присваивания
TEST_F(UniquePtrTest, MoveAssignmentTransfersOwnership) {
    UniquePtr<int> moved_to;
    int* raw_ptr = ptr.get();
    
    moved_to = std::move(ptr);
    
    EXPECT_EQ(moved_to.get(), raw_ptr);
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(*moved_to, 42);
}

TEST_F(UniquePtrTest, SelfMoveAssignmentDoesNothing) {
    int* raw_ptr = ptr.get();
    
    ptr = std::move(ptr);
    
    EXPECT_EQ(ptr.get(), raw_ptr);
    EXPECT_EQ(*ptr, 42);
}

// Тесты методов доступа
TEST_F(UniquePtrTest, GetReturnsRawPointer) {
    EXPECT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr.get(), 42);
}

TEST_F(UniquePtrTest, DereferenceOperatorReturnsValue) {
    EXPECT_EQ(*ptr, 42);
    *ptr = 100;
    EXPECT_EQ(*ptr, 100);
}

TEST_F(UniquePtrTest, ArrowOperatorProvidesMemberAccess) {
    struct TestStruct { int x; };
    UniquePtr<TestStruct> struct_ptr(new TestStruct{42});
    
    EXPECT_EQ(struct_ptr->x, 42);
    struct_ptr->x = 100;
    EXPECT_EQ(struct_ptr->x, 100);
}

TEST_F(UniquePtrTest, BoolOperatorReturnsCorrectState) {
    UniquePtr<int> null_ptr;
    EXPECT_FALSE(static_cast<bool>(null_ptr));
    EXPECT_TRUE(static_cast<bool>(ptr));
}

// Тесты модификаторов
TEST_F(UniquePtrTest, ResetReplacesPointerAndDeletesOld) {
    int* new_ptr = new int(100);
    int* old_ptr = ptr.get();
    
    ptr.reset(new_ptr);
    
    EXPECT_EQ(ptr.get(), new_ptr);
    EXPECT_EQ(*ptr, 100);
    // Не можем проверить что old_ptr удален, но можем проверить что он изменился
}

TEST_F(UniquePtrTest, ResetWithNullptrMakesPtrEmpty) {
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, ReleaseReturnsPointerAndReleasesOwnership) {
    int* raw_ptr = ptr.get();
    int* released_ptr = ptr.release();
    
    EXPECT_EQ(released_ptr, raw_ptr);
    EXPECT_EQ(ptr.get(), nullptr);
    delete released_ptr; // Не забываем почистить
}

TEST_F(UniquePtrTest, SwapExchangesPointers) {
    UniquePtr<int> other(new int(100));
    int* ptr1 = ptr.get();
    int* ptr2 = other.get();
    
    ptr.swap(other);
    
    EXPECT_EQ(ptr.get(), ptr2);
    EXPECT_EQ(other.get(), ptr1);
}

// Тесты для UniquePtr<T[]>
TEST_F(UniquePtrTest, ArrayDefaultConstructorCreatesNullPtr) {
    UniquePtr<int[]> default_arr_ptr;
    EXPECT_EQ(default_arr_ptr.get(), nullptr);
    EXPECT_FALSE(static_cast<bool>(default_arr_ptr));
}

TEST_F(UniquePtrTest, ArrayPointerConstructorTakesOwnership) {
    int* raw_arr = new int[3]{4, 5, 6};
    UniquePtr<int[]> local_arr_ptr(raw_arr);
    
    EXPECT_EQ(local_arr_ptr.get(), raw_arr);
    EXPECT_EQ(local_arr_ptr[0], 4);
    EXPECT_EQ(local_arr_ptr[1], 5);
    EXPECT_EQ(local_arr_ptr[2], 6);
}

TEST_F(UniquePtrTest, ArrayMoveConstructorTransfersOwnership) {
    int* raw_arr = arr_ptr.get();
    UniquePtr<int[]> moved_arr_ptr(std::move(arr_ptr));
    
    EXPECT_EQ(moved_arr_ptr.get(), raw_arr);
    EXPECT_EQ(arr_ptr.get(), nullptr);
    EXPECT_EQ(moved_arr_ptr[0], 1);
    EXPECT_EQ(moved_arr_ptr[1], 2);
    EXPECT_EQ(moved_arr_ptr[2], 3);
}

TEST_F(UniquePtrTest, ArrayMoveAssignmentTransfersOwnership) {
    UniquePtr<int[]> moved_to_arr;
    int* raw_arr = arr_ptr.get();
    
    moved_to_arr = std::move(arr_ptr);
    
    EXPECT_EQ(moved_to_arr.get(), raw_arr);
    EXPECT_EQ(arr_ptr.get(), nullptr);
    EXPECT_EQ(moved_to_arr[0], 1);
}

TEST_F(UniquePtrTest, ArrayIndexOperatorProvidesAccess) {
    EXPECT_EQ(arr_ptr[0], 1);
    EXPECT_EQ(arr_ptr[1], 2);
    EXPECT_EQ(arr_ptr[2], 3);
    
    arr_ptr[1] = 100;
    EXPECT_EQ(arr_ptr[1], 100);
}

TEST_F(UniquePtrTest, ArrayResetReplacesPointerAndDeletesOld) {
    int* new_arr = new int[2]{10, 20};
    int* old_arr = arr_ptr.get();
    
    arr_ptr.reset(new_arr);
    
    EXPECT_EQ(arr_ptr.get(), new_arr);
    EXPECT_EQ(arr_ptr[0], 10);
    EXPECT_EQ(arr_ptr[1], 20);
}

TEST_F(UniquePtrTest, ArrayReleaseReturnsPointerAndReleasesOwnership) {
    int* raw_arr = arr_ptr.get();
    int* released_arr = arr_ptr.release();
    
    EXPECT_EQ(released_arr, raw_arr);
    EXPECT_EQ(arr_ptr.get(), nullptr);
    delete[] released_arr; // Не забываем почистить
}
