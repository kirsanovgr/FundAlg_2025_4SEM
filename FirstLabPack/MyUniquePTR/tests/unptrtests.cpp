#include "../include/unique_ptr.hpp"


#include <iostream>
#include <gtest/gtest.h>

namespace myuniqptr {
    class UniquePtrTest : public ::testing::Test {
    protected:
        static UniquePtr<int> CreateIntPtr() {
            return UniquePtr<int>(new int(13));
        }

        static UniquePtr<int[]> CreateArrayPtr() {
            return UniquePtr<int[]>(new int[3]{1, 2, 3});
        }
    };

    TEST_F(UniquePtrTest, DefaultConstructor) {
        UniquePtr<int> ptr;
        ASSERT_EQ(ptr.get(), nullptr);
        ASSERT_FALSE(static_cast<bool>(ptr));
    }

    TEST_F(UniquePtrTest, MoveSemantics) {
        auto src = CreateIntPtr();
        auto dest = std::move(src);

        ASSERT_NE(dest.get(), nullptr);
        ASSERT_EQ(src.get(), nullptr);
        ASSERT_EQ(*dest, 13);
    }

    TEST_F(UniquePtrTest, MoveAssignment) {
        auto src = CreateIntPtr();
        UniquePtr<int> dest;
        dest = std::move(src);

        ASSERT_EQ(src.get(), nullptr);
        ASSERT_EQ(*dest, 13);
    }

    TEST_F(UniquePtrTest, ResetRelease) {
        auto ptr = CreateIntPtr();
        int* raw = ptr.release();

        ASSERT_EQ(ptr.get(), nullptr);
        ASSERT_EQ(*raw, 13);
        delete raw;
    }

    TEST_F(UniquePtrTest, Swap) {
        auto ptr1 = CreateIntPtr();
        UniquePtr<int> ptr2;

        ptr1.swap(ptr2);

        ASSERT_EQ(ptr1.get(), nullptr);
        ASSERT_EQ(*ptr2, 13);
    }

    class UniquePtrArrayTest : public ::testing::Test {
    protected:
        static UniquePtr<int[]> CreateArray() {
            return UniquePtr<int[]>(new int[3]{1, 2, 3});
        }
    };

    TEST_F(UniquePtrArrayTest, ArrayAccess) {
        auto arr = CreateArray();
        ASSERT_EQ(arr[0], 1);
        ASSERT_EQ(arr[2], 3);
    }

    TEST_F(UniquePtrArrayTest, ArrayMove) {
        auto src = CreateArray();
        UniquePtr<int[]> dest(std::move(src));

        ASSERT_NE(dest.get(), nullptr);
        ASSERT_EQ(src.get(), nullptr);
    }

}
