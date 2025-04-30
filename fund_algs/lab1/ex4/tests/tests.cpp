#include <gtest/gtest.h>
#include "../include/stack.hpp"

using namespace my_container;

class StackTest : public ::testing::Test {
    protected:
        // void SetUp() override {
        //     empty_stack = Stack<int>();
        //     stack1 = Stack<int>({1, 2, 3});
        //     stack2 = Stack<int>({4, 5, 6});
        // }
        Stack<int> empty_stack = Stack<int>();
        Stack<int> stack1 = Stack<int>({1, 2, 3});
        Stack<int> stack2 = Stack<int>({4, 5, 6});
        // Stack<int> empty_stack;
        // Stack<int> stack1;
        // Stack<int> stack2;
};

TEST_F(StackTest, DefaultConstructor) {
    EXPECT_TRUE(empty_stack.empty());
    EXPECT_EQ(empty_stack.size(), 0);
}

TEST_F(StackTest, InitializerListConstructor) {
    
    EXPECT_EQ(stack1.size(), 3);
    EXPECT_EQ(stack1.top(), 3);
}

TEST_F(StackTest, CopyConstructor) {
    Stack<int> copy(stack1);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.top(), 3);
    EXPECT_EQ(stack1.size(), 3);  
}

TEST_F(StackTest, MoveConstructor) {
    Stack<int> moved(std::move(stack1));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.top(), 3);
    EXPECT_TRUE(stack1.empty());  
}

TEST_F(StackTest, CopyAssignment) {
    Stack<int> copy;
    copy = stack1;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.top(), 3);
    EXPECT_EQ(stack1.size(), 3);  
}

TEST_F(StackTest, MoveAssignment) {
    Stack<int> moved;
    moved = std::move(stack1);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.top(), 3);
    EXPECT_TRUE(stack1.empty());  
}

TEST_F(StackTest, Top) {
    EXPECT_EQ(stack1.top(), 3);
    stack1.push(4);
    EXPECT_EQ(stack1.top(), 4);
}

TEST_F(StackTest, TopConst) {
    const Stack<int> const_stack({1, 2, 3});
    EXPECT_EQ(const_stack.top(), 3);
}

TEST_F(StackTest, Empty) {
    EXPECT_TRUE(empty_stack.empty());
    EXPECT_FALSE(stack1.empty());
}

TEST_F(StackTest, Size) {
    EXPECT_EQ(empty_stack.size(), 0);
    EXPECT_EQ(stack1.size(), 3);
}

TEST_F(StackTest, PushLValue) {
    int value = 4;
    stack1.push(value);
    EXPECT_EQ(stack1.top(), 4);
    EXPECT_EQ(stack1.size(), 4);
}

TEST_F(StackTest, PushRValue) {
    stack1.push(4);
    EXPECT_EQ(stack1.top(), 4);
    EXPECT_EQ(stack1.size(), 4);
}

TEST_F(StackTest, Pop) {
    stack1.pop();
    EXPECT_EQ(stack1.top(), 2);
    EXPECT_EQ(stack1.size(), 2);
}

TEST_F(StackTest, PopEmpty) {
    EXPECT_THROW(empty_stack.pop(), std::out_of_range);
}

TEST_F(StackTest, Swap) {
    stack1.swap(stack2);
    EXPECT_EQ(stack1.size(), 3);
    EXPECT_EQ(stack1.top(), 6);
    EXPECT_EQ(stack2.size(), 3);
    EXPECT_EQ(stack2.top(), 3);
}

TEST_F(StackTest, EqualityOperator) {
    Stack<int> stack_copy = stack1;
    EXPECT_TRUE(stack1 == stack_copy);
    EXPECT_FALSE(stack1 == stack2);
}

TEST_F(StackTest, InequalityOperator) {
    EXPECT_TRUE(stack1 != stack2);
    EXPECT_FALSE(stack1 != stack1);
}

TEST_F(StackTest, LessThanOperator) {
    Stack<int> smaller_stack({1, 2});
    EXPECT_TRUE(smaller_stack < stack1);
    EXPECT_FALSE(stack1 < smaller_stack);
}

TEST_F(StackTest, LessThanOrEqualOperator) {
    Stack<int> smaller_stack({1, 2});
    Stack<int> equal_stack = stack1;
    EXPECT_TRUE(smaller_stack <= stack1);
    EXPECT_TRUE(equal_stack <= stack1);
    EXPECT_FALSE(stack1 <= smaller_stack);
}

TEST_F(StackTest, GreaterThanOperator) {
    Stack<int> smaller_stack({1, 2});
    EXPECT_TRUE(stack1 > smaller_stack);
    EXPECT_FALSE(smaller_stack > stack1);
}

TEST_F(StackTest, GreaterThanOrEqualOperator) {
    Stack<int> smaller_stack({1, 2});
    Stack<int> equal_stack = stack1;
    EXPECT_TRUE(stack1 >= smaller_stack);
    EXPECT_TRUE(equal_stack >= stack1);
    EXPECT_FALSE(smaller_stack >= stack1);
}

TEST_F(StackTest, SpaceshipOperator) {
    Stack<int> smaller_stack({1, 2});
    auto result = stack1 <=> smaller_stack;
    EXPECT_TRUE(result > 0);
    result = smaller_stack <=> stack1;
    EXPECT_TRUE(result < 0);
    result = stack1 <=> stack1;
    EXPECT_TRUE(result == 0);
}
