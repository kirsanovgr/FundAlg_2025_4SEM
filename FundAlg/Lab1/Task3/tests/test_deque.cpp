#include <gtest/gtest.h>
#include "../include/deque.hpp"

using namespace my_container;

class DequeTest : public ::testing::Test {
protected:
    Deque<int> d;

    void SetUp() override {
        d.push_back(1);
        d.push_back(2);
        d.push_back(3);
    }
};

TEST_F(DequeTest, DefaultConstructor) {
    Deque<int> temp;
    EXPECT_TRUE(temp.empty());
}

TEST_F(DequeTest, InitListConstructor) {
    Deque<int> temp{10, 20, 30};
    EXPECT_EQ(temp.size(), 3);
    EXPECT_EQ(temp[0], 10);
}

TEST_F(DequeTest, CopyConstructor) {
    Deque<int> copy(d);
    EXPECT_EQ(copy, d);
}

TEST_F(DequeTest, MoveConstructor) {
    Deque<int> temp(std::move(d));
    EXPECT_EQ(temp.size(), 3);
    EXPECT_TRUE(d.empty());
}

TEST_F(DequeTest, CopyAssignment) {
    Deque<int> other;
    other = d;
    EXPECT_EQ(other, d);
}

TEST_F(DequeTest, MoveAssignment) {
    Deque<int> copy(d);
    Deque<int> moved;
    moved = std::move(copy);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(copy.empty());
}

TEST_F(DequeTest, MoveAssignmentFromList) {
    List<int> list;
    list.push_back(10);
    list.push_back(20);

    Deque<int> result;
    result = std::move(list);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 10);
    EXPECT_EQ(result[1], 20);
    EXPECT_TRUE(list.empty());
}

TEST_F(DequeTest, AssignFromContainer) {
    Deque<int> other;
    other.push_back(1);
    other.push_back(2);
    other.push_back(3);

    Container<int> &base = other;
    d = base;

    EXPECT_EQ(d.size(), 3);
}

TEST_F(DequeTest, EqualityOperator) {
    Deque<int> copy(d);
    EXPECT_EQ(copy, d);
}

TEST_F(DequeTest, InequalityOperator) {
    Deque<int> diff{1, 2};
    EXPECT_NE(diff, d);
}

TEST_F(DequeTest, AtValid) {
    EXPECT_EQ(d.at(1), 2);
}

TEST_F(DequeTest, AtInvalid) {
    EXPECT_THROW(d.at(10), std::out_of_range);
}

TEST_F(DequeTest, IndexOperator) {
    EXPECT_EQ(d[2], 3);
    ASSERT_THROW(d[10], std::out_of_range);
}

TEST_F(DequeTest, IteratorTraversal) {
    int sum = 0;
    for (int val: d) sum += val;
    EXPECT_EQ(sum, 6);
}

TEST_F(DequeTest, SizeCheck) {
    EXPECT_EQ(d.size(), 3);
    EXPECT_FALSE(d.empty());

    d.clear();
    EXPECT_EQ(d.size(), 0);
    EXPECT_TRUE(d.empty());
}

TEST_F(DequeTest, SwapTest) {
    Deque<int> other{9, 8, 7};
    d.swap(other);
    EXPECT_EQ(d[0], 9);
    EXPECT_EQ(other[0], 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
