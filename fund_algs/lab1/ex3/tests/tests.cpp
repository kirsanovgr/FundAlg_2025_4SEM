#include "../include/deque.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace {

using namespace my_container;

class DequeTest : public ::testing::Test {
protected:
    Deque<int> emptyDeque;
    Deque<int> sampleDeque{1, 2, 3, 4, 5};
};

// Тесты конструкторов Deque
TEST_F(DequeTest, DefaultConstructor) {
    EXPECT_TRUE(emptyDeque.empty());
    EXPECT_EQ(emptyDeque.size(), 0);
}

TEST_F(DequeTest, InitializerListConstructor) {
    Deque<int> deque{1, 2, 3};
    EXPECT_EQ(deque.size(), 3);
    EXPECT_EQ(deque.front(), 1);
    EXPECT_EQ(deque.back(), 3);
}

TEST_F(DequeTest, CopyConstructor) {
    Deque<int> copy(sampleDeque);
    EXPECT_EQ(copy.size(), sampleDeque.size());
    EXPECT_EQ(copy.front(), sampleDeque.front());
    EXPECT_EQ(copy.back(), sampleDeque.back());
}

TEST_F(DequeTest, MoveConstructor) {
    Deque<int> moved(std::move(sampleDeque));
    EXPECT_EQ(moved.size(), 5);
    EXPECT_TRUE(sampleDeque.empty());
}

// Тесты операторов Deque
TEST_F(DequeTest, CopyAssignment) {
    Deque<int> copy;
    copy = sampleDeque;
    EXPECT_EQ(copy.size(), sampleDeque.size());
    EXPECT_EQ(copy.front(), sampleDeque.front());
    EXPECT_EQ(copy.back(), sampleDeque.back());
}

TEST_F(DequeTest, MoveAssignment) {
    Deque<int> moved;
    moved = std::move(sampleDeque);
    EXPECT_EQ(moved.size(), 5);
    EXPECT_TRUE(sampleDeque.empty());
}

// Тесты методов доступа Deque
TEST_F(DequeTest, FrontAndBack) {
    EXPECT_EQ(sampleDeque.front(), 1);
    EXPECT_EQ(sampleDeque.back(), 5);
}

TEST_F(DequeTest, PushFront) {
    sampleDeque.push_front(0);
    EXPECT_EQ(sampleDeque.front(), 0);
    EXPECT_EQ(sampleDeque.size(), 6);
}

TEST_F(DequeTest, PushBack) {
    sampleDeque.push_back(6);
    EXPECT_EQ(sampleDeque.back(), 6);
    EXPECT_EQ(sampleDeque.size(), 6);
}

TEST_F(DequeTest, PopFront) {
    sampleDeque.pop_front();
    EXPECT_EQ(sampleDeque.front(), 2);
    EXPECT_EQ(sampleDeque.size(), 4);
}

TEST_F(DequeTest, PopBack) {
    sampleDeque.pop_back();
    EXPECT_EQ(sampleDeque.back(), 4);
    EXPECT_EQ(sampleDeque.size(), 4);
}

// Тесты оператора индексации и at()
TEST_F(DequeTest, OperatorBrackets) {
    EXPECT_EQ(sampleDeque[0], 1);
    EXPECT_EQ(sampleDeque[2], 3);
    EXPECT_EQ(sampleDeque[4], 5);
}

TEST_F(DequeTest, AtMethod) {
    EXPECT_EQ(sampleDeque.at(0), 1);
    EXPECT_EQ(sampleDeque.at(2), 3);
    EXPECT_THROW(sampleDeque.at(5), std::out_of_range);
}

// Тесты итераторов Deque
TEST_F(DequeTest, Iterators) {
    int sum = 0;
    for (auto it = sampleDeque.begin(); it != sampleDeque.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST_F(DequeTest, ConstIterators) {
    const Deque<int> constDeque{sampleDeque};
    int sum = 0;
    for (auto it = constDeque.cbegin(); it != constDeque.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST_F(DequeTest, ReverseIterators) {
    int sum = 0;
    for (auto it = sampleDeque.rbegin(); it != sampleDeque.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

// Тесты размерности Deque
TEST_F(DequeTest, SizeAndEmpty) {
    EXPECT_EQ(sampleDeque.size(), 5);
    EXPECT_FALSE(sampleDeque.empty());
    EXPECT_EQ(emptyDeque.size(), 0);
    EXPECT_TRUE(emptyDeque.empty());
}

TEST_F(DequeTest, MaxSize) {
    EXPECT_EQ(sampleDeque.max_size(), sampleDeque.size());
}

TEST_F(DequeTest, Clear) {
    sampleDeque.clear();
    EXPECT_TRUE(sampleDeque.empty());
    EXPECT_EQ(sampleDeque.size(), 0);
}

// Тесты исключений Deque
TEST_F(DequeTest, OutOfRangeExceptions) {
    EXPECT_THROW(emptyDeque.front(), std::out_of_range);
    EXPECT_THROW(emptyDeque.back(), std::out_of_range);
    EXPECT_THROW(emptyDeque.pop_front(), std::out_of_range);
    EXPECT_THROW(emptyDeque.pop_back(), std::out_of_range);
    EXPECT_THROW(emptyDeque.at(0), std::out_of_range);
}

} // namespace