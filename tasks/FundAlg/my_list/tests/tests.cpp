#include <gtest/gtest.h>
#include "container.hpp"

using namespace my_container;

class ListTest : public ::testing::Test {
protected:
    void SetUp() override {
        list_int = List<int>{1, 2, 3};
        list_str = List<std::string>{"one", "two", "three"};
        empty = List<int>();
    }

    List<int> list_int;
    List<std::string> list_str;
    List<int> empty;
};

// Конструкторы
TEST_F(ListTest, DefaultConstructor) {
    List<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST_F(ListTest, InitializerListConstructor) {
    EXPECT_EQ(list_int.size(), 3);
    EXPECT_EQ(list_int.front(), 1);
    EXPECT_EQ(list_int.back(), 3);
}

TEST_F(ListTest, CopyConstructor) {
    List<int> copy(list_int);
    EXPECT_EQ(copy.size(), list_int.size());
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 3);
}

TEST_F(ListTest, MoveConstructor) {
    List<int> temp{10, 20};
    List<int> moved(std::move(temp));
    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(temp.empty());
}

// Операторы присваивания
TEST_F(ListTest, CopyAssignment) {
    List<int> copy;
    copy = list_int;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.front(), 1);
}

TEST_F(ListTest, MoveAssignment) {
    List<int> tmp{5, 6, 7};
    List<int> moved;
    moved = std::move(tmp);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(tmp.empty());
}

// Методы доступа
TEST_F(ListTest, FrontBack) {
    EXPECT_EQ(list_int.front(), 1);
    EXPECT_EQ(list_int.back(), 3);
    EXPECT_EQ(list_str.front(), "one");
}

TEST_F(ListTest, FrontBackThrowsWhenEmpty) {
    EXPECT_THROW(empty.front(), std::out_of_range);
    EXPECT_THROW(empty.back(), std::out_of_range);
}

// Размер
TEST_F(ListTest, SizeAndEmpty) {
    EXPECT_EQ(empty.size(), 0);
    EXPECT_TRUE(empty.empty());
    EXPECT_EQ(list_int.size(), 3);
}

// Изменение
TEST_F(ListTest, PushPop) {
    List<int> l;
    l.push_back(42);
    EXPECT_EQ(l.back(), 42);
    l.push_front(7);
    EXPECT_EQ(l.front(), 7);

    l.pop_back();
    EXPECT_EQ(l.back(), 7);
    l.pop_front();
    EXPECT_TRUE(l.empty());
}

TEST_F(ListTest, Clear) {
    list_int.clear();
    EXPECT_TRUE(list_int.empty());
    EXPECT_EQ(list_int.size(), 0);
}

// Сравнение
TEST_F(ListTest, Equality) {
    List<int> a{1, 2, 3};
    List<int> b{1, 2, 3};
    List<int> c{3, 2, 1};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

// Итераторы
TEST_F(ListTest, Iteration) {
    int expected[] = {1, 2, 3};
    int i = 0;
    for (int val : list_int) {
        EXPECT_EQ(val, expected[i++]);
    }
}
