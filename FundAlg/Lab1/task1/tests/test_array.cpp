#include <gtest/gtest.h>
#include <numeric>

#include "../include/array.h"

using namespace my_container;

class ArrayTest : public ::testing::Test {
protected:
    Array<int, 5> default_array;
    Array<int, 3> init_array = {1, 2, 3};

    void SetUp() override {
        std::iota(default_array.begin(), default_array.end(), 1);
    }
};

TEST_F(ArrayTest, DefaultConstructor) {
    EXPECT_EQ(default_array.size(), 5);
    EXPECT_FALSE(default_array.empty());
    EXPECT_EQ(default_array[0], 1);
    EXPECT_EQ(default_array[4], 5);
}

TEST_F(ArrayTest, InitializerListConstructor) {
    EXPECT_EQ(init_array.size(), 3);
    EXPECT_EQ(init_array[0], 1);
    EXPECT_EQ(init_array[2], 3);

    EXPECT_THROW((Array<int, 2>{1,2,3}), std::invalid_argument);
}

TEST_F(ArrayTest, CopyConstructor) {
    Array<int, 3> copy(init_array);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[2], 3);
}

TEST_F(ArrayTest, MoveConstructor) {
    Array<int, 3> moved(std::move(init_array));
    EXPECT_EQ(moved[0], 1);
}

TEST_F(ArrayTest, AssignmentOperator) {
    Array<int, 5> arr;
    arr = default_array;
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[4], 5);
}

TEST_F(ArrayTest, ElementAccessOperator) {
    default_array[0] = 100;
    EXPECT_EQ(default_array[0], 100);
    EXPECT_EQ(default_array[1], 2);
}

TEST_F(ArrayTest, ComparisonOperators) {
    Array<int, 3> a = {1, 2, 3};
    Array<int, 3> b = {1, 2, 4};
    Array<int, 3> c = {1, 2, 3};

    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(b >= a);
}

TEST_F(ArrayTest, AtMethod) {
    EXPECT_EQ(default_array.at(0), 1);
    EXPECT_THROW(default_array.at(5), std::out_of_range);
}

TEST_F(ArrayTest, FrontBackData) {
    EXPECT_EQ(init_array.front(), 1);
    EXPECT_EQ(init_array.back(), 3);
    EXPECT_EQ(*(init_array.data() + 1), 2);
}

TEST_F(ArrayTest, Iterators) {
    int sum = 0;
    for (auto it = default_array.begin(); it != default_array.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);

    sum = 0;
    for (auto it = default_array.cbegin(); it != default_array.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);

    sum = 0;
    for (auto it = default_array.rbegin(); it != default_array.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST_F(ArrayTest, SizeMethods) {
    EXPECT_EQ(init_array.size(), 3);
    EXPECT_EQ(init_array.max_size(), 3);
    EXPECT_FALSE(init_array.empty());
}

TEST_F(ArrayTest, FillMethod) {
    Array<int, 4> arr;
    arr.fill(42);
    for (auto& item : arr) {
        EXPECT_EQ(item, 42);
    }
}

TEST_F(ArrayTest, SwapMethod) {
    Array<int, 3> a = {1, 2, 3};
    Array<int, 3> b = {4, 5, 6};

    a.swap(b);
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(b[0], 1);
}

TEST(ArrayExceptionTest, InvalidAccess) {
    Array<int, 3> arr = {1, 2, 3};
    EXPECT_NO_THROW(arr.at(2));
    EXPECT_THROW(arr.at(3), std::out_of_range);
}

TEST_F(ArrayTest, ThreeWayComparison) {
    Array<int, 3> a = {1, 2, 3};
    Array<int, 3> b = {1, 2, 4};
    auto result = a <=> b;
    EXPECT_TRUE(result < nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
