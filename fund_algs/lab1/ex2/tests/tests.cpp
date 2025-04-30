#include "../include/list.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace {

using namespace my_container;

class ListTest : public ::testing::Test {
protected:
    List<int> emptyList;
    List<int> sampleList{1, 2, 3, 4, 5};
};

TEST_F(ListTest, DefaultConstructor) {
    EXPECT_TRUE(emptyList.empty());
    EXPECT_EQ(emptyList.size(), 0);
}

TEST_F(ListTest, InitializerListConstructor) {
    List<int> list{1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST_F(ListTest, CopyConstructor) {
    List<int> copy(sampleList);
    EXPECT_EQ(copy.size(), sampleList.size());
    EXPECT_EQ(copy.front(), sampleList.front());
    EXPECT_EQ(copy.back(), sampleList.back());
}

TEST_F(ListTest, MoveConstructor) {
    List<int> moved(std::move(sampleList));
    EXPECT_EQ(moved.size(), 5);
    EXPECT_TRUE(sampleList.empty());
}

TEST_F(ListTest, CopyAssignment) {
    List<int> copy;
    copy = sampleList;
    EXPECT_EQ(copy.size(), sampleList.size());
    EXPECT_EQ(copy.front(), sampleList.front());
    EXPECT_EQ(copy.back(), sampleList.back());
}

TEST_F(ListTest, MoveAssignment) {
    List<int> moved;
    moved = std::move(sampleList);
    EXPECT_EQ(moved.size(), 5);
    EXPECT_TRUE(sampleList.empty());
}

TEST_F(ListTest, FrontAndBack) {
    EXPECT_EQ(sampleList.front(), 1);
    EXPECT_EQ(sampleList.back(), 5);
}

TEST_F(ListTest, PushFront) {
    sampleList.push_front(0);
    EXPECT_EQ(sampleList.front(), 0);
    EXPECT_EQ(sampleList.size(), 6);
}

TEST_F(ListTest, PushBack) {
    sampleList.push_back(6);
    EXPECT_EQ(sampleList.back(), 6);
    EXPECT_EQ(sampleList.size(), 6);
}

TEST_F(ListTest, PopFront) {
    sampleList.pop_front();
    EXPECT_EQ(sampleList.front(), 2);
    EXPECT_EQ(sampleList.size(), 4);
}

TEST_F(ListTest, PopBack) {
    sampleList.pop_back();
    EXPECT_EQ(sampleList.back(), 4);
    EXPECT_EQ(sampleList.size(), 4);
}

TEST_F(ListTest, Insert) {
    auto it = sampleList.begin();
    ++it;
    sampleList.insert(it, 10);
    EXPECT_EQ(sampleList.size(), 6);
    EXPECT_EQ(*++sampleList.begin(), 10);
}

TEST_F(ListTest, EraseSingle) {
    auto it = sampleList.begin();
    ++it;
    sampleList.erase(it);
    EXPECT_EQ(sampleList.size(), 4);
    EXPECT_EQ(*++sampleList.begin(), 3);
}

TEST_F(ListTest, EraseRange) {
    auto first = sampleList.begin();
    ++first;
    auto last = sampleList.end();
    --last;
    sampleList.erase(first, last);
    EXPECT_EQ(sampleList.size(), 2);
    EXPECT_EQ(sampleList.front(), 1);
    EXPECT_EQ(sampleList.back(), 5);
}

TEST_F(ListTest, Clear) {
    sampleList.clear();
    EXPECT_TRUE(sampleList.empty());
    EXPECT_EQ(sampleList.size(), 0);
}

TEST_F(ListTest, ResizeIncrease) {
    sampleList.resize(10, 0);
    EXPECT_EQ(sampleList.size(), 10);
    EXPECT_EQ(sampleList.back(), 0);
}

TEST_F(ListTest, ResizeDecrease) {
    sampleList.resize(3);
    EXPECT_EQ(sampleList.size(), 3);
    EXPECT_EQ(sampleList.back(), 3);
}

TEST_F(ListTest, Swap) {
    List<int> other{6, 7, 8};
    sampleList.swap(other);
    EXPECT_EQ(sampleList.size(), 3);
    EXPECT_EQ(other.size(), 5);
}

TEST_F(ListTest, Iterators) {
    int sum = 0;
    for (auto it = sampleList.begin(); it != sampleList.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);

    sum = 0;
    for (const auto& item : sampleList) {
        sum += item;
    }
    EXPECT_EQ(sum, 15);
}

TEST_F(ListTest, ConstIterators) {
    const List<int> constList{sampleList};
    int sum = 0;
    for (auto it = constList.cbegin(); it != constList.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST_F(ListTest, ReverseIterators) {
    List<int> sampleList{1, 2, 3, 4, 5};
    int sum = 0;
    for (auto it = sampleList.rbegin(); it != sampleList.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST_F(ListTest, ComparisonOperators) {
    List<int> equalList{1, 2, 3, 4, 5};
    List<int> smallerList{1, 2, 3};
    List<int> greaterList{1, 2, 3, 4, 5, 6};

    EXPECT_TRUE(sampleList == equalList);
    EXPECT_TRUE(sampleList != smallerList);
    EXPECT_TRUE(sampleList < greaterList);
    EXPECT_TRUE(sampleList <= equalList);
    EXPECT_TRUE(greaterList > sampleList);
    EXPECT_TRUE(equalList >= sampleList);
}

TEST_F(ListTest, MaxSize) {
    EXPECT_EQ(sampleList.max_size(), sampleList.size());
}

TEST_F(ListTest, Empty) {
    EXPECT_FALSE(sampleList.empty());
    EXPECT_TRUE(emptyList.empty());
}

TEST_F(ListTest, OutOfRangeExceptions) {
    EXPECT_THROW(emptyList.front(), std::out_of_range);
    EXPECT_THROW(emptyList.back(), std::out_of_range);
    EXPECT_THROW(emptyList.pop_front(), std::out_of_range);
    EXPECT_THROW(emptyList.pop_back(), std::out_of_range);
    EXPECT_THROW(emptyList.erase(emptyList.begin()), std::out_of_range);
}

} // namespace