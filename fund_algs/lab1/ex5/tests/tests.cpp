#include "../include/vector.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace my_container;

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty_vec = Vector<int>();
        vec_123 = Vector<int>({1, 2, 3});
        vec_456 = Vector<int>({4, 5, 6});
    }

    Vector<int> empty_vec;
    Vector<int> vec_123;
    Vector<int> vec_456;
};

// Тесты конструкторов
TEST_F(VectorTest, DefaultConstructor) {
    EXPECT_EQ(empty_vec.size(), 0);
    EXPECT_EQ(empty_vec.capacity(), 4);
    EXPECT_TRUE(empty_vec.empty());
}

TEST_F(VectorTest, InitialCapacityConstructor) {
    Vector<int> vec(10);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 10);
    EXPECT_TRUE(vec.empty());
}

TEST_F(VectorTest, InitializerListConstructor) {
    EXPECT_EQ(vec_123.size(), 3);
    EXPECT_GE(vec_123.capacity(), 3);
    EXPECT_FALSE(vec_123.empty());
    EXPECT_EQ(vec_123[0], 1);
    EXPECT_EQ(vec_123[1], 2);
    EXPECT_EQ(vec_123[2], 3);
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> copy(vec_123);
    EXPECT_EQ(copy.size(), vec_123.size());
    EXPECT_EQ(copy.capacity(), vec_123.capacity());
    for (size_t i = 0; i < vec_123.size(); ++i) {
        EXPECT_EQ(copy[i], vec_123[i]);
    }
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> moved(std::move(vec_123));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_GE(moved.capacity(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
    EXPECT_EQ(vec_123.size(), 0);
    EXPECT_EQ(vec_123.capacity(), 0);
}

// Тесты операторов
TEST_F(VectorTest, CopyAssignment) {
    Vector<int> copy;
    copy = vec_123;
    EXPECT_EQ(copy.size(), vec_123.size());
    EXPECT_EQ(copy.capacity(), vec_123.capacity());
    for (size_t i = 0; i < vec_123.size(); ++i) {
        EXPECT_EQ(copy[i], vec_123[i]);
    }
}

TEST_F(VectorTest, MoveAssignment) {
    Vector<int> moved;
    moved = std::move(vec_123);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_GE(moved.capacity(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
    EXPECT_EQ(vec_123.size(), 0);
    EXPECT_EQ(vec_123.capacity(), 4);
}
