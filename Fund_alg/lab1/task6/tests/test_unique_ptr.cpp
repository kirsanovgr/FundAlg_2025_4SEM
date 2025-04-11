#include <gtest/gtest.h>
#include "../include/unique_ptr.hpp"

namespace my_smart_ptr {

    class UniquePtrTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };


    TEST_F(UniquePtrTest, DefaultConstructor) {
        UniquePtr<int> ptr;
        EXPECT_EQ(ptr.get(), nullptr);
    }

    TEST_F(UniquePtrTest, PointerConstructor) {
        int* raw = new int(42);
        UniquePtr<int> ptr(raw);
        EXPECT_EQ(ptr.get(), raw);
        EXPECT_EQ(*ptr, 42);
    }

    TEST_F(UniquePtrTest, MoveConstructor) {
        UniquePtr<int> ptr1(new int(100));
        int* raw = ptr1.get();
        UniquePtr<int> ptr2(std::move(ptr1));
        EXPECT_EQ(ptr2.get(), raw);
        EXPECT_EQ(ptr1.get(), nullptr);
    }

    TEST_F(UniquePtrTest, MoveAssignment) {
        UniquePtr<int> ptr1(new int(200));
        int* raw = ptr1.get();
        UniquePtr<int> ptr2;
        ptr2 = std::move(ptr1);
        EXPECT_EQ(ptr2.get(), raw);
        EXPECT_EQ(ptr1.get(), nullptr);
    }

    TEST_F(UniquePtrTest, Get) {
        int* raw = new int(400);
        UniquePtr<int> ptr(raw);
        EXPECT_EQ(ptr.get(), raw);
    }

    TEST_F(UniquePtrTest, OperatorStar) {
        UniquePtr<int> ptr(new int(500));
        EXPECT_EQ(*ptr, 500);
    }

    TEST_F(UniquePtrTest, OperatorBool) {
        UniquePtr<int> ptr1;
        EXPECT_FALSE(ptr1);
        ptr1.reset(new int(600));
        EXPECT_TRUE(ptr1);
    }

    TEST_F(UniquePtrTest, Reset) {
        UniquePtr<int> ptr(new int(700));
        ptr.reset(new int(800));
        EXPECT_EQ(*ptr, 800);
        ptr.reset();
        EXPECT_EQ(ptr.get(), nullptr);
    }

    TEST_F(UniquePtrTest, Release) {
        int* raw = new int(900);
        UniquePtr<int> ptr(raw);
        int* released = ptr.release();
        EXPECT_EQ(released, raw);
        EXPECT_EQ(ptr.get(), nullptr);
        delete released;
    }

    TEST_F(UniquePtrTest, Swap) {
        UniquePtr<int> ptr1(new int(1000));
        UniquePtr<int> ptr2(new int(2000));
        int* raw1 = ptr1.get();
        int* raw2 = ptr2.get();
        ptr1.swap(ptr2);
        EXPECT_EQ(ptr1.get(), raw2);
        EXPECT_EQ(ptr2.get(), raw1);
    }

    TEST_F(UniquePtrTest, ArrayConstructor) {
        UniquePtr<int[]> ptr(new int[3]{1, 2, 3});
        EXPECT_NE(ptr.get(), nullptr);
        EXPECT_EQ(ptr[0], 1);
        EXPECT_EQ(ptr[1], 2);
        EXPECT_EQ(ptr[2], 3);
    }

    TEST_F(UniquePtrTest, ArrayReset) {
        UniquePtr<int[]> ptr(new int[2]{4, 5});
        ptr.reset(new int[3]{6, 7, 8});
        EXPECT_EQ(ptr[0], 6);
        EXPECT_EQ(ptr[1], 7);
        EXPECT_EQ(ptr[2], 8);
    }


    struct Deletable {
        static int delete_count;
        ~Deletable() { delete_count++; }
    };
    int Deletable::delete_count = 0;

    TEST_F(UniquePtrTest, DestructorReleasesResource) {
        {
            UniquePtr<Deletable> ptr(new Deletable);
            EXPECT_EQ(Deletable::delete_count, 0);
        }
        EXPECT_EQ(Deletable::delete_count, 1);
    }

    struct DeletableArray {
        static int delete_count;
        ~DeletableArray() { delete_count++; }
    };
    int DeletableArray::delete_count = 0;

    TEST_F(UniquePtrTest, ArrayDestructorCallsDeleteArray) {
        {
            UniquePtr<DeletableArray[]> ptr(new DeletableArray[2]);
            EXPECT_EQ(DeletableArray::delete_count, 0);
        }
        EXPECT_EQ(DeletableArray::delete_count, 2);
    }

}
