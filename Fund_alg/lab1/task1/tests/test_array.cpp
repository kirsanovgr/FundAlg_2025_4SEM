#include <gtest/gtest.h>

#include "../include/array.hpp"

namespace my_container {


    class ArrayTest : public ::testing::Test {
    protected:
        using IntArray5 = Array<int, 5>;
        IntArray5 default_array;
        IntArray5 init_array{1, 2, 3, 4, 5};
        IntArray5 another_array{6, 7, 8, 9, 10};

        void SetUp() override {
        }
    };

    template<typename T, size_t N>
    void test_array_content(const Array<T, N> &arr, const std::vector<T> &expected) {
        for (size_t i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(arr[i], expected[i]);
        }
    }

    TEST_F(ArrayTest, DefaultConstructor) {
        test_array_content(default_array, {0, 0, 0, 0, 0});
    }

    TEST_F(ArrayTest, InitializerListConstructor) {
        test_array_content(init_array, {1, 2, 3, 4, 5});
        Array<int, 5> short_init{1, 2};
        test_array_content(short_init, {1, 2, 0, 0, 0});

        EXPECT_THROW((my_container::Array<char, 2> {'a', 'b', 'c'}), std::invalid_argument);

        short_init = {1 , 2, 3};
        test_array_content(short_init, {1, 2, 3, 0, 0});

        EXPECT_THROW((short_init = {1, 2, 3, 4, 5, 6, 7}), std::invalid_argument);


    }

    TEST_F(ArrayTest, CopyConstructor) {
        IntArray5 copy(init_array);
        test_array_content(copy, {1, 2, 3, 4, 5});
    }

    TEST_F(ArrayTest, MoveConstructor) {
        IntArray5 moved(std::move(init_array));
        test_array_content(moved, {1, 2, 3, 4, 5});
    }

    TEST_F(ArrayTest, AssignmentOperator) {
        default_array = init_array;
        test_array_content(default_array, {1, 2, 3, 4, 5});
        default_array = default_array;
        test_array_content(default_array, {1, 2, 3, 4, 5});
    }

    TEST(ArrayMoveAssignmentTest, BasicMoveAssignment) {
        my_container::Array<int, 4> src = {1, 2, 3, 4};
        my_container::Array<int, 4> dest;

        dest = std::move(src);

        EXPECT_EQ(dest[0], 1);
        EXPECT_EQ(dest[1], 2);
        EXPECT_EQ(dest[2], 3);
        EXPECT_EQ(dest[3], 4);

        for (const auto &item: src) {
            EXPECT_EQ(item, 0);
        }
    }

    TEST_F(ArrayTest, SubscriptOperator) {
        init_array[0] = 10;
        EXPECT_EQ(init_array[0], 10);
        const auto &const_ref = init_array;
        EXPECT_EQ(const_ref[1], 2);
    }

    TEST_F(ArrayTest, ComparisonOperators) {
        IntArray5 arr1{1, 2, 3, 4, 5};
        IntArray5 arr2{1, 2, 3, 4, 5};
        IntArray5 arr3{2, 1, 3, 4, 5};

        EXPECT_EQ(arr1, arr2);
        EXPECT_NE(arr1, arr3);
        EXPECT_LT(arr1, arr3);
        EXPECT_GT(arr3, arr1);
    }

    TEST_F(ArrayTest, SwapMethod) {
        init_array.swap(another_array);
        test_array_content(init_array, {6, 7, 8, 9, 10});
        test_array_content(another_array, {1, 2, 3, 4, 5});
    }

    TEST_F(ArrayTest, AtMethod) {
        EXPECT_EQ(init_array.at(0), 1);
        EXPECT_THROW(init_array.at(5), std::out_of_range);
        const auto &const_ref = init_array;
        EXPECT_EQ(const_ref.at(4), 5);
        EXPECT_THROW(const_ref.at(5), std::out_of_range);
    }

    TEST_F(ArrayTest, FrontBackMethods) {
        EXPECT_EQ(init_array.front(), 1);
        EXPECT_EQ(init_array.back(), 5);
        const auto &const_ref = init_array;
        EXPECT_EQ(const_ref.front(), 1);
        EXPECT_EQ(const_ref.back(), 5);
    }

    TEST_F(ArrayTest, DataMethod) {
        int *data = init_array.data();
        EXPECT_EQ(data[0], 1);
        const auto &const_ref = init_array;
        const int *cdata = const_ref.data();
        EXPECT_EQ(cdata[4], 5);
    }

    TEST_F(ArrayTest, Iterators) {
        size_t i = 0;
        for (auto it = init_array.begin(); it != init_array.end(); ++it, ++i) {
            EXPECT_EQ(*it, init_array[i]);
        }
        i = 0;
        for (auto it = init_array.cbegin(); it != init_array.cend(); ++it, ++i) {
            EXPECT_EQ(*it, init_array[i]);
        }
    }

    TEST_F(ArrayTest, ReverseIterators) {
        auto rit = init_array.rbegin();
        EXPECT_EQ(*rit, 5);
        ++rit;
        EXPECT_EQ(*rit, 4);
        const auto &const_ref = init_array;
        auto crit = const_ref.crbegin();
        EXPECT_EQ(*crit, 5);
    }

    TEST_F(ArrayTest, SizeMethods) {
        EXPECT_EQ(init_array.size(), 5);
        EXPECT_EQ(init_array.max_size(), 5);
        EXPECT_FALSE(init_array.empty());
    }

    TEST_F(ArrayTest, FillMethod) {
        init_array.fill(42);
        test_array_content(init_array, {42, 42, 42, 42, 42});
    }

    TEST_F(ArrayTest, AtOutOfRange) {
        EXPECT_THROW(default_array.at(5), std::out_of_range);
        const auto &const_ref = default_array;
        EXPECT_THROW(const_ref.at(5), std::out_of_range);
    }

}

template<typename T>
class DummyContainer : public Container<T> {
public:
    DummyContainer() = default;

    Container<T> &operator=(const Container<T> &) override { return *this; }

    bool operator==(const Container<T> &) const override { return false; }

    bool operator!=(const Container<T> &) const override { return true; }

    size_t size() const override { return 0; }

    size_t max_size() const override { return 0; }

    bool empty() const override { return true; }
};

TEST(ArrayAssignmentTest, ValidSameTypeAssignment) {
    my_container::Array<int, 3> src = {1, 2, 3};
    my_container::Array<int, 3> dest;

    ASSERT_NO_THROW(dest = src);

    EXPECT_EQ(dest[0], 1);
    EXPECT_EQ(dest[1], 2);
    EXPECT_EQ(dest[2], 3);
}

TEST(ArrayAssignmentTest, InvalidTypeAssignment) {
    my_container::Array<int, 3> arr;
    DummyContainer<int> dummy;

    ASSERT_THROW(arr = dummy, std::invalid_argument);
}

TEST(ArrayAssignmentTest, DifferentSizeAssignment) {
    my_container::Array<int, 3> dest;
    my_container::Array<int, 5> different_size;

    ASSERT_THROW(dest = different_size, std::invalid_argument);
}

TEST(ArrayAssignmentTest, SelfAssignment) {
    my_container::Array<int, 3> arr = {5, 6, 7};
    const Container<int> &self = arr;

    ASSERT_NO_THROW(arr = self);

    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 6);
    EXPECT_EQ(arr[2], 7);
}

TEST(ArrayAssignmentTest, PartialModificationCheck) {
    my_container::Array<int, 4> src = {10, 20, 30, 40};
    my_container::Array<int, 4> dest = {0, 0, 0, 0};

    dest = src;
    src[1] = 99;

    EXPECT_EQ(dest[1], 20);
    EXPECT_EQ(src[1], 99);
}


TEST(ArrayAccessorsTest, FrontBackConstMethods) {
    my_container::Array<int, 4> arr = {10, 20, 30, 40};
    const auto &const_arr = arr;

    EXPECT_EQ(const_arr.front(), 10);
    EXPECT_EQ(arr.front(), 10);

    EXPECT_EQ(const_arr.back(), 40);
    EXPECT_EQ(arr.back(), 40);

    arr.front() = 100;
    arr.back() = 400;
    EXPECT_EQ(const_arr.front(), 100);
    EXPECT_EQ(const_arr.back(), 400);
}

TEST(ArrayIteratorsTest, ReverseIteratorsBehavior) {
    my_container::Array<char, 5> arr = {'a', 'b', 'c', 'd', 'e'};
    const auto &const_arr = arr;

    std::string reverse_result;
    for (auto rit = arr.rbegin(); rit != arr.rend(); ++rit) {
        reverse_result += *rit;
    }
    EXPECT_EQ(reverse_result, "edcba");

    std::string const_reverse_result;
    for (auto crit = const_arr.crbegin(); crit != const_arr.crend(); ++crit) {
        const_reverse_result += *crit;
    }
    EXPECT_EQ(const_reverse_result, "edcba");
}

TEST(ArrayIteratorsTest, ReverseIteratorManipulation) {
    my_container::Array<int, 3> arr = {1, 2, 3};

    auto rit = arr.rbegin();
    ASSERT_NE(rit, arr.rend());
    EXPECT_EQ(*rit, 3);

    ++rit;
    EXPECT_EQ(*rit, 2);

    rit += 1;
    EXPECT_EQ(*rit, 1);

    ++rit;
    EXPECT_EQ(rit, arr.rend());
}

TEST(ArrayEdgeCasesTest, SingleElementArray) {
    my_container::Array<std::string, 1> arr = {"test"};
    const auto &const_arr = arr;

    EXPECT_EQ(arr.front(), "test");
    EXPECT_EQ(arr.back(), "test");
    EXPECT_EQ(*arr.rbegin(), "test");
    EXPECT_EQ(*const_arr.crbegin(), "test");
}
