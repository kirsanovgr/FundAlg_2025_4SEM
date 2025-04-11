#include <gtest/gtest.h>
#include "../include/array.hpp"

using namespace my_container;

class ArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        arr1 = Array<int, 3>{1, 2, 3};
        arr2 = Array<int, 3>{4, 5, 6};
    }

    Array<int, 3> arr1;
    Array<int, 3> arr2;
    Array<int, 3> arrCopy;
};

TEST_F(ArrayTest, DefaultConstructor) {
    Array<int, 3> arr;
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 0);
    }
}

TEST_F(ArrayTest, InitializerListConstructor) {
    EXPECT_EQ(arr1[0], 1);
    EXPECT_EQ(arr1[1], 2);
    EXPECT_EQ(arr1[2], 3);

    EXPECT_THROW((Array<int, 2>{1, 2, 3}), std::invalid_argument);
}

TEST_F(ArrayTest, CopyConstructor) {
    Array<int, 3> copy(arr1);
    EXPECT_TRUE(std::equal(copy.begin(), copy.end(), arr1.begin()));
}

TEST_F(ArrayTest, MoveConstructor) {
    Array<int, 3> moved(std::move(arr1));
    EXPECT_TRUE(std::equal(moved.begin(), moved.end(), Array<int, 3>{1, 2, 3}.begin()));
}

TEST_F(ArrayTest, CopyAssignment) {
    arrCopy = arr1;
    EXPECT_TRUE(std::equal(arrCopy.begin(), arrCopy.end(), arr1.begin()));
}

TEST_F(ArrayTest, MoveAssignment) {
    Array<int, 3> moved;
    moved = std::move(arr1);
    EXPECT_TRUE(std::equal(moved.begin(), moved.end(), Array<int, 3>{1, 2, 3}.begin()));
}

TEST_F(ArrayTest, EqualityOperator) {
    Array<int, 3> same{1, 2, 3};
    EXPECT_TRUE(arr1 == same);
    EXPECT_FALSE(arr1 == arr2);
}

TEST_F(ArrayTest, InequalityOperator) {
    EXPECT_TRUE(arr1 != arr2);
}

TEST_F(ArrayTest, ThreeWayComparison) {
    Array<int, 3> smaller{1, 2, 2};
    EXPECT_EQ(arr1 <=> smaller, std::strong_ordering::greater);
    EXPECT_EQ(arr1 <=> arr1, std::strong_ordering::equal);
}


TEST_F(ArrayTest, AtMethod) {
    EXPECT_EQ(arr1.at(0), 1);
    EXPECT_THROW(arr1.at(3), std::out_of_range);
}

TEST_F(ArrayTest, OperatorBrackets) {
    arr1[1] = 10;
    EXPECT_EQ(arr1[1], 10);
}

TEST_F(ArrayTest, FrontAndBack) {
    EXPECT_EQ(arr1.front(), 1);
    EXPECT_EQ(arr1.back(), 3);
}

TEST_F(ArrayTest, DataMethod) {
    int *data = arr1.data();
    EXPECT_EQ(data[0], 1);
}

TEST_F(ArrayTest, Iterators) {
    size_t index = 0;
    for (auto it = arr1.begin(); it != arr1.end(); ++it) {
        EXPECT_EQ(*it, arr1[index++]);
    }
}

TEST_F(ArrayTest, ReverseIterators) {
    size_t index = arr1.size() - 1;
    for (auto it = arr1.rbegin(); it != arr1.rend(); ++it) {
        EXPECT_EQ(*it, arr1[index--]);
    }
}

TEST_F(ArrayTest, SizeMethods) {
    EXPECT_EQ(arr1.size(), 3);
    EXPECT_EQ(arr1.max_size(), 3);
    EXPECT_FALSE(arr1.empty());
}

TEST_F(ArrayTest, FillMethod) {
    arr1.fill(42);
    for (auto elem: arr1) {
        EXPECT_EQ(elem, 42);
    }
}

TEST_F(ArrayTest, SwapMethod) {
    arr1.swap(arr2);
    EXPECT_TRUE(std::equal(arr1.begin(), arr1.end(), Array<int, 3>{4, 5, 6}.begin()));
    EXPECT_TRUE(std::equal(arr2.begin(), arr2.end(), Array<int, 3>{1, 2, 3}.begin()));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
