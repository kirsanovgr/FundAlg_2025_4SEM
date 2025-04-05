#include "../include/deque.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>

using namespace my_container;

class DequeTest : public ::testing::Test {
protected:
    Deque<int> emptyDeque;
    Deque<int> deque3 = {1, 2, 3};
    Deque<int> deque5 = {10, 20, 30, 40, 50};

    void SetUp() override {
        deque3 = {1, 2, 3};
        deque5 = {10, 20, 30, 40, 50};
    }
};

TEST_F(DequeTest, MoveConstructor) {
    Deque<int> moved(std::move(deque5));
    EXPECT_EQ(moved.size(), 5);
    EXPECT_TRUE(deque5.empty());
    EXPECT_EQ(moved.front(), 10);
}

TEST_F(DequeTest, SelfAssignment) {
    deque5 = deque5;
    EXPECT_EQ(deque5.size(), 5);
    EXPECT_EQ(deque5.back(), 50);
}

TEST_F(DequeTest, AssignmentFromDifferentSize) {
    Deque<int> copy = {100};
    copy = deque5;
    EXPECT_EQ(copy.size(), 5);
    EXPECT_EQ(copy[4], 50);
}

TEST_F(DequeTest, AtMethodEdgeCases) {
    EXPECT_EQ(deque5.at(0), 10);
    EXPECT_EQ(deque5.at(4), 50);
    EXPECT_THROW(deque5.at(5), std::out_of_range);
    EXPECT_THROW(deque5.at(-1), std::out_of_range);
}

TEST_F(DequeTest, OperatorSubscriptModification) {
    deque5[2] = 300;
    EXPECT_EQ(deque5[2], 300);
    deque5[4] = deque5[0];
    EXPECT_EQ(deque5[4], 10);
}

TEST_F(DequeTest, ConstIterators) {
    const Deque<int>& constDeque = deque5;
    std::vector<int> result;
    for (auto it = constDeque.cbegin(); it != constDeque.cend(); ++it) {
        result.push_back(*it);
    }
    EXPECT_EQ(result, std::vector<int>({10, 20, 30, 40, 50}));
}

TEST_F(DequeTest, MaxSizeConsistency) {
    EXPECT_EQ(emptyDeque.max_size(), deque5.max_size());
}

TEST_F(DequeTest, InsertEraseComplex) {
    Deque<int> d = {1, 3, 4};
    auto it = d.begin();
    ++it;
    it = d.insert(it, 2);
    EXPECT_EQ(d, Deque<int>({1, 2, 3, 4}));

    it = d.erase(it);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(d, Deque<int>({1, 3, 4}));
}

TEST_F(DequeTest, ResizeWithValues) {
    deque3.resize(5, 100);
    EXPECT_EQ(deque3, Deque<int>({1, 2, 3, 100, 100}));

    deque3.resize(2);
    EXPECT_EQ(deque3, Deque<int>({1, 2}));
}

TEST_F(DequeTest, SwapWithEmpty) {
    Deque<int> empty;
    deque5.swap(empty);
    EXPECT_TRUE(deque5.empty());
    EXPECT_EQ(empty.size(), 5);
}

TEST_F(DequeTest, ComparisonsWithDifferentSizes) {
    Deque<int> shorter = {10, 20};
    Deque<int> greater = {10, 20, 30, 40, 50, 60};

    EXPECT_TRUE(deque5 < greater);
    EXPECT_TRUE(deque5 > shorter);
    EXPECT_TRUE(deque5 != shorter);
}

TEST_F(DequeTest, RangeBasedForLoop) {
    std::vector<int> values;
    for (const auto& item : deque5) {
        values.push_back(item);
    }
    EXPECT_EQ(values, std::vector<int>({10, 20, 30, 40, 50}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
