#include <gtest/gtest.h>
#include <stdexcept>
#include "../include/stack.h"

using namespace my_container;

class StackTest : public ::testing::Test {
protected:
    Stack<int> emptyStack;
    Stack<int> sampleStack = {1, 2, 3};

    void SetUp() override {
        sampleStack = {1, 2, 3};
    }
};

TEST_F(StackTest, DefaultConstructor) {
    EXPECT_TRUE(emptyStack.empty());
    EXPECT_EQ(emptyStack.size(), 0);
}

TEST_F(StackTest, InitializerListConstructor) {
    Stack<int> stack = {4, 5, 6};
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), 6);
}

TEST_F(StackTest, CopyConstructor) {
    Stack<int> copy(sampleStack);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.top(), 3);
}

TEST_F(StackTest, MoveConstructor) {
    Stack<int> moved(std::move(sampleStack));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(sampleStack.empty());
}

TEST_F(StackTest, CopyAssignment) {
    Stack<int> copy;
    copy = sampleStack;
    EXPECT_EQ(copy.top(), 3);
}

TEST_F(StackTest, MoveAssignment) {
    Stack<int> moved;
    moved = std::move(sampleStack);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(sampleStack.empty());
}

TEST_F(StackTest, TopAccess) {
    EXPECT_EQ(sampleStack.top(), 3);
}

TEST_F(StackTest, TopException) {
    EXPECT_THROW(emptyStack.top(), std::out_of_range);
}

TEST_F(StackTest, PushPop) {
    sampleStack.push(4);
    EXPECT_EQ(sampleStack.top(), 4);
    sampleStack.pop();
    EXPECT_EQ(sampleStack.top(), 3);
}

TEST_F(StackTest, Swap) {
    Stack<int> other = {10, 20};
    sampleStack.swap(other);
    EXPECT_EQ(sampleStack.size(), 2);
    EXPECT_EQ(other.size(), 3);
}

TEST_F(StackTest, Comparisons) {
    Stack<int> same = {1, 2, 3};
    Stack<int> smaller = {1, 2};

    EXPECT_TRUE(sampleStack == same);
    EXPECT_TRUE(sampleStack > smaller);
    EXPECT_TRUE(smaller < sampleStack);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}