#include <gtest/gtest.h>
#include "../include/array.hpp"

using namespace my_container;

// Тестирование конструкторов
TEST(ArrayConstructors, DefaultConstructor) {
    Array<int, 3> arr;
    EXPECT_EQ(arr.size(), 3);
    for (const auto& elem : arr) {
        EXPECT_EQ(elem, int{});
    }
}

TEST(ArrayConstructors, InitializerListConstructor) {
    // Полная инициализация
    Array<int, 3> arr1 = {1, 2, 3};
    EXPECT_EQ(arr1[0], 1);
    EXPECT_EQ(arr1[1], 2);
    EXPECT_EQ(arr1[2], 3);

    // Частичная инициализация
    Array<int, 5> arr2 = {1, 2};
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    for (size_t i = 2; i < 5; ++i) {
        EXPECT_EQ(arr2[i], int{});
    }
}

TEST(ArrayConstructors, CopyConstructor) {
    Array<int, 3> original = {1, 2, 3};
    Array<int, 3> copy(original);
    
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

TEST(ArrayConstructors, MoveConstructor) {
    Array<int, 3> original = {1, 2, 3};
    Array<int, 3> moved(std::move(original));
    
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
}

// Тестирование операторов
TEST(ArrayOperators, CopyAssignment) {
    Array<int, 3> original = {1, 2, 3};
    Array<int, 3> copy;
    copy = original;
    
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

TEST(ArrayOperators, MoveAssignment) {
    Array<int, 3> original = {1, 2, 3};
    Array<int, 3> moved;
    moved = std::move(original);
    
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
}

TEST(ArrayOperators, SubscriptOperator) {
    Array<int, 3> arr = {1, 2, 3};
    
    // Проверка чтения
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    
    // Проверка записи
    arr[1] = 5;
    EXPECT_EQ(arr[1], 5);
}

TEST(ArrayOperators, ConstSubscriptOperator) {
    const Array<int, 3> arr = {1, 2, 3};
    
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(ArrayOperators, EqualityOperators) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2 = {1, 2, 3};
    Array<int, 3> arr3 = {4, 5, 6};
    
    EXPECT_EQ(arr1, arr2);
    EXPECT_NE(arr1, arr3);
}

TEST(ArrayOperators, ComparisonOperators) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2 = {1, 2, 4};
    Array<int, 3> arr3 = {0, 2, 3};
    
    EXPECT_LT(arr1, arr2);
    EXPECT_LE(arr1, arr2);
    EXPECT_GT(arr1, arr3);
    EXPECT_GE(arr1, arr3);
}

TEST(ArrayOperators, Swap) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2 = {4, 5, 6};
    
    arr1.swap(arr2);
    
    EXPECT_EQ(arr1[0], 4);
    EXPECT_EQ(arr1[1], 5);
    EXPECT_EQ(arr1[2], 6);
    
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);
}

// Тестирование методов доступа
TEST(ArrayAccessMethods, At) {
    Array<int, 3> arr = {1, 2, 3};
    
    EXPECT_EQ(arr.at(0), 1);
    EXPECT_EQ(arr.at(1), 2);
    EXPECT_EQ(arr.at(2), 3);
    
    EXPECT_THROW(arr.at(3), std::out_of_range);
}

TEST(ArrayAccessMethods, ConstAt) {
    const Array<int, 3> arr = {1, 2, 3};
    
    EXPECT_EQ(arr.at(0), 1);
    EXPECT_EQ(arr.at(1), 2);
    EXPECT_EQ(arr.at(2), 3);
    
    EXPECT_THROW(arr.at(3), std::out_of_range);
}

TEST(ArrayAccessMethods, FrontBack) {
    Array<int, 3> arr = {1, 2, 3};
    
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 3);
    
    arr.front() = 10;
    arr.back() = 30;
    
    EXPECT_EQ(arr.front(), 10);
    EXPECT_EQ(arr.back(), 30);
}

TEST(ArrayAccessMethods, ConstFrontBack) {
    const Array<int, 3> arr = {1, 2, 3};
    
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 3);
}

TEST(ArrayAccessMethods, Data) {
    Array<int, 3> arr = {1, 2, 3};
    int* ptr = arr.data();
    
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
}

TEST(ArrayAccessMethods, ConstData) {
    const Array<int, 3> arr = {1, 2, 3};
    const int* ptr = arr.data();
    
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
}

// Тестирование итераторов
TEST(ArrayIterators, BeginEnd) {
    Array<int, 3> arr = {1, 2, 3};
    
    auto it = arr.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, arr.end());
}

TEST(ArrayIterators, CbeginCend) {
    const Array<int, 3> arr = {1, 2, 3};
    
    auto it = arr.cbegin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, arr.cend());
}

TEST(ArrayIterators, RbeginRend) {
    Array<int, 3> arr = {1, 2, 3};
    
    auto rit = arr.rbegin();
    EXPECT_EQ(*rit, 3);
    ++rit;
    EXPECT_EQ(*rit, 2);
    ++rit;
    EXPECT_EQ(*rit, 1);
    ++rit;
    EXPECT_EQ(rit, arr.rend());
}

TEST(ArrayIterators, CrbeginCrend) {
    const Array<int, 3> arr = {1, 2, 3};
    
    auto rit = arr.crbegin();
    EXPECT_EQ(*rit, 3);
    ++rit;
    EXPECT_EQ(*rit, 2);
    ++rit;
    EXPECT_EQ(*rit, 1);
    ++rit;
    EXPECT_EQ(rit, arr.crend());
}

// Тестирование размерности
TEST(ArrayCapacity, Size) {
    Array<int, 5> arr;
    EXPECT_EQ(arr.size(), 5);
    
    Array<int, 0> empty_arr;
    EXPECT_EQ(empty_arr.size(), 0);
}

TEST(ArrayCapacity, MaxSize) {
    Array<int, 5> arr;
    EXPECT_EQ(arr.max_size(), 5);
    
    Array<int, 0> empty_arr;
    EXPECT_EQ(empty_arr.max_size(), 0);
}

TEST(ArrayCapacity, Empty) {
    Array<int, 0> empty_arr;
    EXPECT_TRUE(empty_arr.empty());
    
    Array<int, 3> non_empty_arr;
    EXPECT_FALSE(non_empty_arr.empty());
}

// Тестирование метода fill
TEST(ArrayFill, Fill) {
    Array<int, 3> arr;
    arr.fill(42);
    
    for (const auto& elem : arr) {
        EXPECT_EQ(elem, 42);
    }
    
    // Проверка, что все элементы заполнены
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[1], 42);
    EXPECT_EQ(arr[2], 42);
}