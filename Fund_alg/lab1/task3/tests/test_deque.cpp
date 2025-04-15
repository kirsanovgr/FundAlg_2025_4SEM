#include <gtest/gtest.h>
#include <stdexcept>
#include "../include/deque.hpp"

namespace my_container {
    class DequeTest : public ::testing::Test {
    protected:
        Deque<int> dq;

        void SetUp() override {
            dq.push_back(1);
            dq.push_back(2);
            dq.push_back(3);
        }
    };


    TEST_F(DequeTest, DefaultConstructor) {
        Deque<int> empty_dq;
        EXPECT_TRUE(empty_dq.empty());
        EXPECT_EQ(empty_dq.size(), 0);
    }

    TEST_F(DequeTest, InitializerListConstructor) {
        Deque<int> init_dq{10, 20, 30};
        ASSERT_EQ(init_dq.size(), 3);
        EXPECT_EQ(init_dq[0], 10);
        EXPECT_EQ(init_dq[2], 30);
    }

    TEST_F(DequeTest, CopyConstructor) {
        Deque<int> copy(dq);
        ASSERT_EQ(copy.size(), dq.size());
        for (size_t i = 0; i < dq.size(); ++i) {
            EXPECT_EQ(copy[i], dq[i]);
        }
    }

    TEST_F(DequeTest, MoveConstructor) {
        Deque<int> moved(std::move(dq));
        EXPECT_TRUE(dq.empty());
        ASSERT_EQ(moved.size(), 3);
        EXPECT_EQ(moved.front(), 1);
        EXPECT_EQ(moved.back(), 3);
    }


    TEST_F(DequeTest, CopyAssignment) {
        Deque<int> copy;
        copy = dq;
        ASSERT_EQ(copy.size(), dq.size());
        EXPECT_EQ(copy.front(), 1);
        EXPECT_EQ(copy.back(), 3);
    }

    TEST_F(DequeTest, MoveAssignment) {
        Deque<int> moved;
        moved = std::move(dq);
        EXPECT_TRUE(dq.empty());
        ASSERT_EQ(moved.size(), 3);
        EXPECT_EQ(moved[1], 2);
    }


    TEST_F(DequeTest, AtValid) {
        EXPECT_EQ(dq.at(0), 1);
        EXPECT_EQ(dq.at(2), 3);
    }

    TEST_F(DequeTest, AtInvalid) {
        EXPECT_THROW(dq.at(3), std::out_of_range);
    }

    TEST_F(DequeTest, OperatorBrackets) {
        dq[1] = 99;
        EXPECT_EQ(dq[0], 1);
        EXPECT_EQ(dq[1], 99);
        EXPECT_EQ(dq[2], 3);
    }

    TEST_F(DequeTest, FrontBack) {
        EXPECT_EQ(dq.front(), 1);
        EXPECT_EQ(dq.back(), 3);
    }

    TEST_F(DequeTest, BeginEnd) {
        int sum = 0;
        for (auto it = dq.begin(); it != dq.end(); ++it) {
            sum += *it;
        }
        EXPECT_EQ(sum, 6);
    }

    TEST_F(DequeTest, ReverseIterators) {
        Deque<int> reversed;
        for (auto it = dq.rbegin(); it != dq.rend(); ++it) {
            reversed.push_back(*it);
        }
        EXPECT_EQ(reversed[0], 3);
        EXPECT_EQ(reversed[2], 1);
    }

    TEST_F(DequeTest, EmptySize) {
        Deque<int> empty_dq;
        EXPECT_TRUE(empty_dq.empty());
        EXPECT_EQ(empty_dq.size(), 0);

        EXPECT_FALSE(dq.empty());
        EXPECT_EQ(dq.size(), 3);
    }

    TEST_F(DequeTest, MaxSize) {
        EXPECT_GT(dq.max_size(), 0);
    }

    TEST_F(DequeTest, PushPopFrontBack) {
        dq.push_front(0);
        EXPECT_EQ(dq.front(), 0);
        dq.pop_front();
        EXPECT_EQ(dq.front(), 1);

        dq.push_back(4);
        EXPECT_EQ(dq.back(), 4);
        dq.pop_back();
        EXPECT_EQ(dq.back(), 3);
    }


    TEST_F(DequeTest, Clear) {
        dq.clear();
        EXPECT_TRUE(dq.empty());
        EXPECT_EQ(dq.size(), 0);
    }

    TEST_F(DequeTest, Resize) {
        dq.resize(5);
        ASSERT_EQ(dq.size(), 5);
        EXPECT_EQ(dq[3], 0);

        dq.resize(2);
        ASSERT_EQ(dq.size(), 2);
        EXPECT_EQ(dq.back(), 2);
    }

    TEST_F(DequeTest, Swap) {
        Deque<int> other{9, 8, 7, 6};
        dq.swap(other);

        EXPECT_EQ(dq.size(), 4);
        EXPECT_EQ(other.size(), 3);
        EXPECT_EQ(dq.front(), 9);
        EXPECT_EQ(other.back(), 3);
    }

    TEST_F(DequeTest, Comparisons) {
        Deque<int> d1{1, 2, 3};
        Deque<int> d2{1, 2, 4};

        EXPECT_TRUE(d1 < d2);
        EXPECT_FALSE(d1 == d2);
        EXPECT_TRUE(d1 <=> d2 == std::strong_ordering::less);
    }
}
// namespace my_container
