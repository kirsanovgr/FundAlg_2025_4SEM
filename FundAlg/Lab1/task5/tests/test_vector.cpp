#include <gtest/gtest.h>
#include "../include/vector.h"

using namespace my_container;

class VectorTest : public ::testing::Test {
protected:
    Vector<int> emptyVec;
    Vector<int> sampleVec = {1, 2, 3, 4, 5};

    void SetUp() override {
        sampleVec = {1, 2, 3, 4, 5};
    }
};

// Конструкторы
TEST_F(VectorTest, DefaultConstructor) {
    EXPECT_TRUE(emptyVec.empty());
    EXPECT_EQ(emptyVec.size(), 0);
    EXPECT_EQ(emptyVec.capacity(), 0);
}

TEST_F(VectorTest, InitializerListConstructor) {
    Vector<int> vec = {10, 20, 30};
    EXPECT_EQ(vec.size(), 3);
    EXPECT_GE(vec.capacity(), 3);
    EXPECT_EQ(vec[2], 30);
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> copy(sampleVec);
    EXPECT_EQ(copy.size(), 5);
    EXPECT_EQ(copy.capacity(), 5);
    EXPECT_EQ(copy[4], 5);
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> moved(std::move(sampleVec));
    EXPECT_EQ(moved.size(), 5);
    EXPECT_TRUE(sampleVec.empty());
}

// Операторы присваивания
TEST_F(VectorTest, CopyAssignment) {
    Vector<int> copy;
    copy = sampleVec;
    EXPECT_EQ(copy.size(), 5);
    EXPECT_EQ(copy[4], 5);
}

TEST_F(VectorTest, MoveAssignment) {
    Vector<int> moved;
    moved = std::move(sampleVec);
    EXPECT_EQ(moved.size(), 5);
    EXPECT_TRUE(sampleVec.empty());
}

// Методы доступа
TEST_F(VectorTest, AtMethod) {
    EXPECT_EQ(sampleVec.at(2), 3);
    EXPECT_THROW(sampleVec.at(10), std::out_of_range);
}

TEST_F(VectorTest, FrontBackData) {
    EXPECT_EQ(sampleVec.front(), 1);
    EXPECT_EQ(sampleVec.back(), 5);
    sampleVec.front() = 10;
    sampleVec.back() = 50;
    EXPECT_EQ(sampleVec[0], 10);
    EXPECT_EQ(sampleVec[4], 50);
    EXPECT_NE(sampleVec.data(), nullptr);
}

// Емкость
TEST_F(VectorTest, ReserveCapacity) {
    emptyVec.reserve(10);
    EXPECT_GE(emptyVec.capacity(), 10);
    EXPECT_TRUE(emptyVec.empty());
}

TEST_F(VectorTest, ShrinkToFit) {
    sampleVec.reserve(20);
    sampleVec.shrink_to_fit();
    EXPECT_EQ(sampleVec.capacity(), 5);
}

// Модификаторы
TEST_F(VectorTest, PushBackReallocation) {
    sampleVec.push_back(6);
    EXPECT_EQ(sampleVec.size(), 6);
    EXPECT_EQ(sampleVec[5], 6);
}

TEST_F(VectorTest, InsertErase) {
    sampleVec.insert(2, 10);
    EXPECT_EQ(sampleVec.size(), 6);
    EXPECT_EQ(sampleVec[2], 10);

    sampleVec.erase(3);
    EXPECT_EQ(sampleVec.size(), 5);
    EXPECT_EQ(sampleVec[3], 4);
}

TEST_F(VectorTest, Resize) {
    sampleVec.resize(3);
    EXPECT_EQ(sampleVec.size(), 3);
    sampleVec.resize(5, 100);
    EXPECT_EQ(sampleVec[4], 100);
}

TEST_F(VectorTest, Swap) {
    Vector<int> other = {10, 20};
    sampleVec.swap(other);
    EXPECT_EQ(sampleVec.size(), 2);
    EXPECT_EQ(other.size(), 5);
}

// Сравнения
TEST_F(VectorTest, Comparisons) {
    Vector<int> same = {1, 2, 3, 4, 5};
    Vector<int> greater = {1, 2, 3, 4, 6};
    EXPECT_TRUE(sampleVec == same);
    EXPECT_TRUE(sampleVec < greater);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}