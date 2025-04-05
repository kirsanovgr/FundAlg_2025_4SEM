#include <gtest/gtest.h>
#include <stdexcept>
#include <initializer_list>
#include "../include/list.h"

using namespace my_container;

class ListTest : public ::testing::Test {
protected:
    List<int> emptyList;
    List<int> sampleList = {1, 2, 3};
    List<int> singleElementList = {42};

    void SetUp() override {
        sampleList = {1, 2, 3};
    }
};

TEST_F(ListTest, DefaultConstructor) {
    EXPECT_TRUE(emptyList.empty());
    EXPECT_EQ(emptyList.size(), 0);
}

TEST_F(ListTest, InitializerListConstructor) {
    List<int> list = {4, 5, 6};
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 4);
    EXPECT_EQ(list.back(), 6);
}

TEST_F(ListTest, CopyConstructor) {
    List<int> copy(sampleList);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 3);
    EXPECT_EQ(sampleList.size(), 3);
}

TEST_F(ListTest, MoveConstructor) {
    List<int> moved(std::move(sampleList));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(sampleList.empty());
}

TEST_F(ListTest, CopyAssignment) {
    List<int> copy;
    copy = sampleList;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.back(), 3);
}

TEST_F(ListTest, MoveAssignment) {
    List<int> moved;
    moved = std::move(sampleList);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(sampleList.empty());
}

TEST_F(ListTest, FrontBackAccess) {
    EXPECT_EQ(sampleList.front(), 1);
    EXPECT_EQ(sampleList.back(), 3);
    sampleList.front() = 10;
    sampleList.back() = 30;
    EXPECT_EQ(sampleList.front(), 10);
    EXPECT_EQ(sampleList.back(), 30);
}

TEST_F(ListTest, FrontBackExceptions) {
    EXPECT_THROW(emptyList.front(), std::out_of_range);
    EXPECT_THROW(emptyList.back(), std::out_of_range);
}

TEST_F(ListTest, Iterators) {
    int sum = 0;
    for (auto it = sampleList.begin(); it != sampleList.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);

    sum = 0;
    for (const auto& item : sampleList) {
        sum += item;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(ListTest, ConstIterators) {
    const List<int> constList = sampleList;
    int sum = 0;
    for (auto it = constList.begin(); it != constList.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(ListTest, SizeEmpty) {
    EXPECT_EQ(sampleList.size(), 3);
    EXPECT_FALSE(sampleList.empty());
    EXPECT_EQ(emptyList.size(), 0);
    EXPECT_TRUE(emptyList.empty());
}

TEST_F(ListTest, MaxSize) {
    EXPECT_EQ(sampleList.max_size(), std::numeric_limits<size_t>::max());
}

TEST_F(ListTest, Clear) {
    sampleList.clear();
    EXPECT_TRUE(sampleList.empty());
    EXPECT_EQ(sampleList.size(), 0);
}

TEST_F(ListTest, Insert) {
    auto it = sampleList.begin();
    ++it;
    sampleList.insert(it, 10);
    EXPECT_EQ(sampleList.size(), 4);
    EXPECT_EQ(sampleList.front(), 1);
    EXPECT_EQ(*it, 2);
}

TEST_F(ListTest, Erase) {
    auto it = sampleList.begin();
    ++it;
    sampleList.erase(it);
    EXPECT_EQ(sampleList.size(), 2);
    EXPECT_EQ(sampleList.front(), 1);
    EXPECT_EQ(sampleList.back(), 3);
}

TEST_F(ListTest, PushPopBack) {
    sampleList.push_back(4);
    EXPECT_EQ(sampleList.back(), 4);
    sampleList.pop_back();
    EXPECT_EQ(sampleList.back(), 3);
}

TEST_F(ListTest, PushPopFront) {
    sampleList.push_front(0);
    EXPECT_EQ(sampleList.front(), 0);
    sampleList.pop_front();
    EXPECT_EQ(sampleList.front(), 1);
}

TEST_F(ListTest, Resize) {
    sampleList.resize(5);
    EXPECT_EQ(sampleList.size(), 5);
    EXPECT_EQ(sampleList.back(), 0);

    sampleList.resize(2);
    EXPECT_EQ(sampleList.size(), 2);
    EXPECT_EQ(sampleList.back(), 2);
}

TEST_F(ListTest, Swap) {
    List<int> other = {4, 5, 6};
    sampleList.swap(other);
    EXPECT_EQ(sampleList.size(), 3);
    EXPECT_EQ(sampleList.front(), 4);
    EXPECT_EQ(other.size(), 3);
    EXPECT_EQ(other.front(), 1);
}

TEST_F(ListTest, Comparisons) {
    List<int> equal = {1, 2, 3};
    List<int> greater = {1, 2, 4};
    List<int> shorter = {1, 2};

    EXPECT_TRUE(sampleList == equal);
    EXPECT_FALSE(sampleList != equal);
    EXPECT_TRUE(sampleList < greater);
    EXPECT_TRUE(sampleList <= greater);
    EXPECT_TRUE(greater > sampleList);
    EXPECT_TRUE(greater >= sampleList);
    EXPECT_TRUE(sampleList > shorter);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}