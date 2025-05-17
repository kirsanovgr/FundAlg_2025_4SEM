//
// Created by Олеся on 11.05.2025.
//
#include <gtest/gtest.h>
#include "stack.hpp"

class StackTest : public ::testing::Test {
protected:
    Stack<int> empty_stack;
    Stack<int> one_stack{1};
    Stack<int> multi_stack{1, 2, 3};

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(StackTest, DefaultConstructor) {
    EXPECT_TRUE(empty_stack.empty());
    EXPECT_EQ(empty_stack.size(), 0);
}

TEST_F(StackTest, InitializerListConstructor) {
    EXPECT_FALSE(multi_stack.empty());
    EXPECT_EQ(multi_stack.size(), 3);
    EXPECT_EQ(multi_stack.top(), 1);
}

TEST_F(StackTest, CopyConstructor) {
    Stack<int> copy = multi_stack;
    EXPECT_EQ(copy, multi_stack);
}

TEST_F(StackTest, MoveConstructor) {
    Stack<int> moved = std::move(multi_stack);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(multi_stack.empty());
}

TEST_F(StackTest, CopyAssignment) {
    Stack<int> s;
    s = one_stack;
    EXPECT_EQ(s.size(), 1);
    EXPECT_EQ(s.top(), 1);
    EXPECT_EQ(s, one_stack);
}

TEST_F(StackTest, MoveAssignment) {
    Stack<int> s;
    s = std::move(one_stack);
    EXPECT_EQ(s.size(), 1);
    EXPECT_EQ(s.top(), 1);
    EXPECT_TRUE(one_stack.empty());
}

TEST_F(StackTest, PushPopTop) {
    empty_stack.push(42);
    EXPECT_EQ(empty_stack.top(), 42);
    EXPECT_EQ(empty_stack.size(), 1);

    empty_stack.push(100);
    EXPECT_EQ(empty_stack.top(), 100);
    EXPECT_EQ(empty_stack.size(), 2);

    empty_stack.pop();
    EXPECT_EQ(empty_stack.top(), 42);
    EXPECT_EQ(empty_stack.size(), 1);

    empty_stack.pop();
    EXPECT_TRUE(empty_stack.empty());
}

TEST_F(StackTest, PopThrowsIfEmpty) {
    EXPECT_THROW(empty_stack.pop(), std::out_of_range);
}

TEST_F(StackTest, TopThrowsIfEmpty) {
    EXPECT_THROW(empty_stack.top(), std::out_of_range);
}

TEST_F(StackTest, SwapTest) {
    Stack<int> s1{1, 2};
    Stack<int> s2{9, 8};
    s1.swap(s2);

    EXPECT_EQ(s1.top(), 9);
    EXPECT_EQ(s2.top(), 1);
}

TEST_F(StackTest, EqualityComparison) {
    Stack<int> a{1, 2, 3};
    Stack<int> b{1, 2, 3};
    Stack<int> c{3, 2, 1};

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST_F(StackTest, ComparisonOperators) {
    Stack<int> a{1, 2};
    Stack<int> b{1, 2, 3};
    Stack<int> c{1, 2};

    EXPECT_LT(a, b);
    EXPECT_LE(a, b);
    EXPECT_LE(a, c);
    EXPECT_GT(b, a);
    EXPECT_GE(b, a);
    EXPECT_GE(c, a);
}
