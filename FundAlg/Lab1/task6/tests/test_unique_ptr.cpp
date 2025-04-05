#include <gtest/gtest.h>
#include "../include/unique_ptr.h"

using namespace my_smart_ptr;

struct TestObject {
    int value;
    TestObject(int v) : value(v) {}
};

class UniquePtrTest : public ::testing::Test {
protected:
    UniquePtr<TestObject> emptyPtr;
    UniquePtr<TestObject> objPtr;

    void SetUp() override {
        objPtr = UniquePtr<TestObject>(new TestObject(42));
    }
};

TEST_F(UniquePtrTest, DefaultConstructor) {
    EXPECT_EQ(emptyPtr.get(), nullptr);
    EXPECT_FALSE(static_cast<bool>(emptyPtr));
}

TEST_F(UniquePtrTest, MoveConstructor) {
    UniquePtr<TestObject> moved(std::move(objPtr));
    EXPECT_EQ(objPtr.get(), nullptr);
    EXPECT_NE(moved.get(), nullptr);
    EXPECT_EQ(moved->value, 42);
}

TEST_F(UniquePtrTest, MoveAssignment) {
    UniquePtr<TestObject> moved;
    moved = std::move(objPtr);
    EXPECT_EQ(objPtr.get(), nullptr);
    EXPECT_EQ(moved->value, 42);
}

TEST_F(UniquePtrTest, Reset) {
    objPtr.reset(new TestObject(100));
    EXPECT_EQ(objPtr->value, 100);
    objPtr.reset();
    EXPECT_EQ(objPtr.get(), nullptr);
}

TEST_F(UniquePtrTest, Release) {
    TestObject* rawPtr = objPtr.release();
    EXPECT_EQ(objPtr.get(), nullptr);
    EXPECT_EQ(rawPtr->value, 42);
    delete rawPtr;
}

TEST_F(UniquePtrTest, Swap) {
    UniquePtr<TestObject> other(new TestObject(200));
    objPtr.swap(other);
    EXPECT_EQ(objPtr->value, 200);
    EXPECT_EQ(other->value, 42);
}

class UniquePtrArrayTest : public ::testing::Test {
protected:
    UniquePtr<int[]> emptyArray;
    UniquePtr<int[]> sampleArray;

    void SetUp() override {
        sampleArray = UniquePtr<int[]>(new int[3]{1, 2, 3});
    }
};

TEST_F(UniquePtrArrayTest, ArrayAccess) {
    EXPECT_EQ(sampleArray[0], 1);
    sampleArray[1] = 4;
    EXPECT_EQ(sampleArray[1], 4);
}

TEST_F(UniquePtrArrayTest, ResetArray) {
    sampleArray.reset(new int[2]{10, 20});
    EXPECT_EQ(sampleArray[0], 10);
    sampleArray.reset();
    EXPECT_EQ(sampleArray.get(), nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}