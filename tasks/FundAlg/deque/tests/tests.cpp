//
// Created by Олеся on 09.05.2025.
//
#include <gtest/gtest.h>
#include "deque.hpp"


class DequeTest : public ::testing::Test {
protected:
    Deque<int> dq;

    void SetUp() override {
        dq = {10, 20, 30};
    }
};

TEST_F(DequeTest, SizeAndAccess) {
    EXPECT_EQ(dq.size(), 3);
    EXPECT_EQ(dq.front(), 10);
    EXPECT_EQ(dq.back(), 30);
}

TEST_F(DequeTest, AtOperatorValid) {
    EXPECT_EQ(dq.at(0), 10);
    EXPECT_EQ(dq.at(1), 20);
    EXPECT_EQ(dq.at(2), 30);
}

TEST_F(DequeTest, AtOperatorOutOfRange) {
    EXPECT_THROW(dq.at(3), std::out_of_range);
    EXPECT_THROW(dq.at(999), std::out_of_range);
}

TEST_F(DequeTest, IndexOperator) {
    EXPECT_EQ(dq[0], 10);
    EXPECT_EQ(dq[1], 20);
    EXPECT_EQ(dq[2], 30);
}

TEST_F(DequeTest, PushBackAndPushFront) {
    dq.push_back(40);
    dq.push_front(5);
    EXPECT_EQ(dq.front(), 5);
    EXPECT_EQ(dq.back(), 40);
    EXPECT_EQ(dq.size(), 5);
}

TEST_F(DequeTest, PopBackAndPopFront) {
    dq.pop_back();
    dq.pop_front();
    EXPECT_EQ(dq.size(), 1);
    EXPECT_EQ(dq.front(), 20);
}

TEST_F(DequeTest, ClearDeque) {
    dq.clear();
    EXPECT_TRUE(dq.empty());
    EXPECT_EQ(dq.size(), 0);
}

TEST_F(DequeTest, CopyConstructor) {
    Deque<int> copy(dq);
    EXPECT_EQ(copy, dq);
}

TEST_F(DequeTest, MoveConstructor) {
    Deque<int> moved(std::move(dq));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(dq.empty());
}

TEST_F(DequeTest, CopyAssignment) {
    Deque<int> other;
    other = dq;
    EXPECT_EQ(other, dq);
}

TEST_F(DequeTest, MoveAssignment) {
    Deque<int> other;
    other = std::move(dq);
    EXPECT_EQ(other.size(), 3);
    EXPECT_TRUE(dq.empty());
}
