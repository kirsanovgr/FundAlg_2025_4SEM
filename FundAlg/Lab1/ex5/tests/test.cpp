#include <gtest/gtest.h>

#include "../include/vector.h"

namespace my_container::test {
class VectorTest : public ::testing::Test {
   protected:
	void SetUp() override {
		vec_int = {1, 2, 3};
		vec_str = {"a", "b", "c"};
	}

	Vector<int> vec_int;
	Vector<std::string> vec_str;
	Vector<int> empty_vec;
};

TEST_F(VectorTest, DefaultConstructor) {
	EXPECT_EQ(empty_vec.size(), 0);
	EXPECT_EQ(empty_vec.capacity(), 10);
}

TEST_F(VectorTest, CopyConstructor) {
	Vector<int> copy(vec_int);
	EXPECT_EQ(copy.size(), 3);
	EXPECT_EQ(copy[0], 1);
}

TEST_F(VectorTest, MoveConstructor) {
	Vector<int> moved(std::move(vec_int));
	EXPECT_EQ(moved.size(), 3);
	EXPECT_TRUE(vec_int.empty());
}

TEST_F(VectorTest, InitializerListConstructor) {
	Vector init_list{1, 2, 3};
	EXPECT_EQ(init_list.size(), 3);
}

TEST_F(VectorTest, CopyAssignment) {
	Vector<int> copy;
	copy = vec_int;
	EXPECT_EQ(copy.size(), 3);
	EXPECT_EQ(copy[1], 2);
}


TEST_F(VectorTest, ElementAccess) {
	EXPECT_EQ(vec_int[0], 1);
	vec_int[1] = 42;
	EXPECT_EQ(vec_int[1], 42);
	EXPECT_EQ(vec_int.at(2), 3);
	EXPECT_THROW(vec_int.at(5), std::out_of_range);
}

TEST_F(VectorTest, FrontBackData) {
	EXPECT_EQ(vec_int.front(), 1);
	EXPECT_EQ(vec_int.back(), 3);
	EXPECT_EQ(*vec_int.data(), 1);
}

TEST_F(VectorTest, CapacityMethods) {
	EXPECT_FALSE(vec_int.empty());
	vec_int.reserve(10);
	EXPECT_GE(vec_int.capacity(), 10);
	vec_int.shrink_to_fit();
	EXPECT_EQ(vec_int.capacity(), 3);
}

TEST_F(VectorTest, PushPop) {
	vec_int.push_back(4);
	EXPECT_EQ(vec_int.size(), 4);
	vec_int.pop_back();
	EXPECT_EQ(vec_int.size(), 3);
}

TEST_F(VectorTest, InsertErase) {
	vec_int.insert(vec_int.data() + 1, 42);
	EXPECT_EQ(vec_int[1], 42);
	EXPECT_EQ(vec_int.size(), 4);
	vec_int.erase(vec_int.data());
	EXPECT_EQ(vec_int[0], 42);
	EXPECT_EQ(vec_int.size(), 3);
}

TEST_F(VectorTest, ClearResize) {
	vec_int.clear();
	EXPECT_TRUE(vec_int.empty());
	vec_int.resize(5);
	EXPECT_EQ(vec_int.size(), 5);
}

TEST_F(VectorTest, Comparisons) {
	Vector<int> v1{1, 2, 3};
	Vector<int> v2{1, 2, 4};
	Vector<int> v3{1, 2};

	EXPECT_TRUE(v1 == vec_int);
	EXPECT_TRUE(v1 != v2);
	EXPECT_TRUE(v1 < v2);
	EXPECT_TRUE(v3 <= v1);
	EXPECT_TRUE(v2 > v1);
	EXPECT_TRUE(v1 >= v3);
}

TEST_F(VectorTest, SwapTest) {
	Vector<int> v1{1, 2};
	Vector<int> v2{3, 4, 5};
	v1.swap(v2);
	EXPECT_EQ(v1.size(), 3);
	EXPECT_EQ(v2.size(), 2);
}

TEST_F(VectorTest, EdgeCases) {
	Vector<int> small_vec;
	small_vec.push_back(1);
	small_vec.push_back(2);
	small_vec.insert(small_vec.data(), 0);
	EXPECT_EQ(small_vec.capacity(), 10);
}

TEST_F(VectorTest, MoveSemantics) {
	Vector<std::string> moved(std::move(vec_str));
	EXPECT_TRUE(vec_str.empty());
	EXPECT_EQ(moved[0], "a");
}
}  