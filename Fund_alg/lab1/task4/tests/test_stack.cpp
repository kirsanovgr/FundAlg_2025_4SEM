#include <gtest/gtest.h>
#include "../include/stack.hpp"

namespace my_container {

    class StackTest : public ::testing::Test {
    protected:
        Stack<int> empty_stack;
        Stack<int> sample_stack{3, 2, 1};
        Stack<std::string> str_stack{"hello", "world"};

        void SetUp() override {
            while (!sample_stack.empty()) sample_stack.pop();
            sample_stack.push(1);
            sample_stack.push(2);
            sample_stack.push(3);
        }
    };

    TEST_F(StackTest, EmptyStackOperations) {
        EXPECT_THROW(empty_stack.top(), std::out_of_range);
        EXPECT_THROW(empty_stack.pop(), std::out_of_range);
    }
    TEST_F(StackTest, PushOrder) {
        Stack<std::string> s;
        s.push("a");
        s.push("b");
        s.push("c");

        EXPECT_EQ(s.top(), "c");
        s.pop();
        EXPECT_EQ(s.top(), "b");
    }
    TEST_F(StackTest, MoveAssignmentWithNonEmptyTarget) {
        Stack<int> temp{5, 6};
        sample_stack = std::move(temp);
        EXPECT_EQ(sample_stack.top(), 5);
        EXPECT_TRUE(temp.empty());
    }

    TEST_F(StackTest, MultiplePushPopOperations) {
        sample_stack.push(4);
        sample_stack.push(5);

        EXPECT_EQ(sample_stack.top(), 5);

        sample_stack.pop();
        EXPECT_EQ(sample_stack.top(), 4);

        sample_stack.pop();
        EXPECT_EQ(sample_stack.top(), 3);
    }

    TEST_F(StackTest, ClearViaPop) {
        while (!sample_stack.empty()) {
            sample_stack.pop();
        }
        EXPECT_TRUE(sample_stack.empty());
        EXPECT_THROW(sample_stack.pop(), std::out_of_range);
    }

    TEST_F(StackTest, AllComparisonOperators) {
        Stack<int> s1{1, 2, 3};
        Stack<int> s2{1, 2, 4};
        Stack<int> s3{1, 2};

        EXPECT_TRUE((s1 <=> s2) == std::strong_ordering::less);
        EXPECT_TRUE((s2 <=> s1) == std::strong_ordering::greater);
        EXPECT_TRUE((s1 <=> s3) == std::strong_ordering::greater);

        EXPECT_TRUE(s2 > s1);
        EXPECT_TRUE(s3 < s1);
        EXPECT_TRUE(s1 >= s3);
    }

    TEST_F(StackTest, SelfAssignment) {
        sample_stack = sample_stack;
        EXPECT_EQ(sample_stack.size(), 3);
        EXPECT_EQ(sample_stack.top(), 3);
    }

    TEST_F(StackTest, SwapWithEmpty) {
        Stack<int> empty;
        sample_stack.swap(empty);
        EXPECT_TRUE(sample_stack.empty());
        EXPECT_EQ(empty.size(), 3);
    }

    TEST_F(StackTest, LargeStack) {
        Stack<int> large_stack;
        for (int i = 0; i < 1000; ++i) {
            large_stack.push(i);
        }
        EXPECT_EQ(large_stack.top(), 999);
        EXPECT_EQ(large_stack.size(), 1000);
    }

    TEST_F(StackTest, ComplexTypeOperations) {
        Stack<std::vector<int>> vec_stack;
        vec_stack.push({1, 2, 3});
        vec_stack.push({4, 5});

        EXPECT_EQ(vec_stack.top().size(), 2);
        vec_stack.top().push_back(6);
        EXPECT_EQ(vec_stack.top().back(), 6);
    }

    TEST_F(StackTest, ContainerMethods) {
        EXPECT_EQ(sample_stack.size(), 3);
        EXPECT_FALSE(sample_stack.empty());
    }

}
