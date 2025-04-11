#include <gtest/gtest.h>
#include "../include/vector.hpp"

using namespace my_container;

TEST(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.capacity(), 2);
}

TEST(VectorTest, InitializerListConstructor) {
    Vector<int> v{1, 2, 3};
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, CopyConstructor) {
    Vector<int> original{1, 2, 3};
    Vector<int> copy(original);
    ASSERT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
}

TEST(VectorTest, MoveConstructor) {
    Vector<int> original{1, 2, 3};
    Vector<int> moved(std::move(original));

    ASSERT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);

    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);
}

TEST(VectorTest, AssignmentOperator) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2;
    v2 = v1;

    ASSERT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TEST(VectorTest, MoveAssignmentOperator) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2;
    v2 = std::move(v1);

    ASSERT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);

    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);
}

TEST(VectorTest, ElementAccess) {
    Vector<int> v{10, 20, 30};

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v.at(1), 20);
    EXPECT_THROW(v.at(3), std::out_of_range);

    v[1] = 200;
    EXPECT_EQ(v[1], 200);
}

TEST(VectorTest, FrontBackData) {
    Vector<int> v{5, 10, 15};

    EXPECT_EQ(v.front(), 5);
    EXPECT_EQ(v.back(), 15);
    EXPECT_EQ(*v.data(), 5);

    v.front() = 1;
    v.back() = 3;
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, PushBack) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, PopBack) {
    Vector<int> v{1, 2, 3};
    v.pop_back();
    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);
}

TEST(VectorTest, Reserve) {
    Vector<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, Resize) {
    Vector<int> v;
    v.resize(5, 42);
    ASSERT_EQ(v.size(), 5);
    for (auto item : v) {
        EXPECT_EQ(item, 42);
    }

    v.resize(3);
    ASSERT_EQ(v.size(), 3);
}

TEST(VectorTest, InsertIntoEmpty) {
    Vector<int> v;

    auto it = v.insert(v.begin(), 42);
    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(*it, 42);
    EXPECT_EQ(v[0], 42);
}

TEST(VectorTest, InsertAtBeginWithReallocation) {
    Vector<int> v{2, 3}; // capacity = 2
    v.insert(v.begin(), 1);
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, InsertAtBeginWithoutReallocation) {
    Vector<int> v;
    v.reserve(3);
    v.push_back(2);
    v.push_back(3);
    v.insert(v.begin(), 1); // Не вызывает realloc
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, InsertAtEnd)   {
    Vector<int> v{1, 2};
    Vector<int>::iterator it = v.insert(v.end() , 3);
    EXPECT_EQ(v, Vector<int>({1, 2, 3}));
    EXPECT_EQ(*it, 3);
}

TEST(VectorTest, Erase) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 1, v.begin() + 3);
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v, Vector<int>({1, 4, 5}));
    EXPECT_EQ(*it, 4);
}

TEST(VectorTest, Clear) {
    Vector<int> v{1, 2, 3};
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, ShrinkToFit) {
    Vector<int> v;
    v.reserve(10);
    v.push_back(1);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 1);
}

TEST(VectorTest, Iterators) {
    Vector<int> v{1, 2, 3};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(VectorTest, Comparisons) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 4};
    Vector<int> v3{1, 2, 3};

    EXPECT_TRUE(v1 == v3);
    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v1 < v2);
    EXPECT_TRUE(v2 > v1);
}

TEST(VectorTest, Exceptions) {
    Vector<int> v;
    EXPECT_THROW(v.at(0), std::out_of_range);
    EXPECT_THROW(v.front(), std::out_of_range);
    EXPECT_THROW(v.back(), std::out_of_range);
}
