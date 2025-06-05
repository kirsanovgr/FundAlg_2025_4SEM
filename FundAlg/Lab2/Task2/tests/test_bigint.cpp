#include "../include/bigint.hpp"
#include <gtest/gtest.h>
#include <random>
#include <climits>

// Тестирование конструкторов
TEST(BigIntTest, DefaultConstructor) {
	BigInt a;
	EXPECT_EQ(a, BigInt(0));
}

TEST(BigIntTest, ConstructorFromLongLong) {
	EXPECT_EQ(BigInt(0), BigInt(0));
	EXPECT_EQ(BigInt(123), BigInt("123"));
	EXPECT_EQ(BigInt(-456), BigInt("-456"));
	EXPECT_EQ(BigInt(999999999), BigInt("999999999"));
}

TEST(BigIntTest, ConstructorFromString) {
	EXPECT_EQ(BigInt("0"), BigInt(0));
	EXPECT_EQ(BigInt("123"), BigInt(123));
	EXPECT_EQ(BigInt("-456"), BigInt(-456));
	EXPECT_EQ(BigInt("000123"), BigInt(123));
	EXPECT_EQ(BigInt("-000123"), BigInt(-123));
	EXPECT_EQ(BigInt("1000000"), BigInt(1000000));
	EXPECT_EQ(BigInt("12345678901234567890"), BigInt("12345678901234567890"));

	// Исключения при некорректных строках
	EXPECT_THROW(BigInt(""), std::invalid_argument);
	EXPECT_THROW(BigInt("-"), std::invalid_argument);
	EXPECT_THROW(BigInt("abc"), std::invalid_argument);
	EXPECT_THROW(BigInt("123a"), std::invalid_argument);
	EXPECT_THROW(BigInt("-abc"), std::invalid_argument);
}

TEST(BigIntTest, SelfAssignment) {
	BigInt a("12345678901234567890");
	BigInt original = a;
	a = a;

	EXPECT_EQ(a, original);
	EXPECT_EQ(a, BigInt("12345678901234567890"));
}

TEST(BigIntTest, MoveSelfAssignment) {
	BigInt a("9876543210");
	BigInt original = a;
	a = std::move(a);
	EXPECT_EQ(a, original);
	EXPECT_EQ(a, BigInt("9876543210"));
}

TEST(BigIntTest, CopyAndMoveOperations) {
	BigInt a(123);
	BigInt b(a);
	EXPECT_EQ(a, b);

	BigInt c(-456);
	b = c;
	EXPECT_EQ(b, c);

	BigInt d(std::move(a));
	EXPECT_EQ(d, BigInt(123));
	EXPECT_EQ(a, BigInt(0));

	BigInt e;
	e = std::move(b);
	EXPECT_EQ(e, BigInt(-456));
	EXPECT_EQ(b, BigInt(0));
}

// Тестирование арифметических операций
TEST(BigIntTest, Addition) {
	EXPECT_EQ(BigInt(123) + BigInt(456), BigInt(579));
	EXPECT_EQ(BigInt(-123) + BigInt(456), BigInt(333));
	EXPECT_EQ(BigInt(123) + BigInt(-456), BigInt(-333));
	EXPECT_EQ(BigInt(-123) + BigInt(-456), BigInt(-579));
	EXPECT_EQ(BigInt(99) + BigInt(1), BigInt(100));
	EXPECT_EQ(BigInt(0) + BigInt(0), BigInt(0));
}

TEST(BigIntTest, Subtraction) {
	EXPECT_EQ(BigInt(456) - BigInt(123), BigInt(333));
	EXPECT_EQ(BigInt(123) - BigInt(456), BigInt(-333));
	EXPECT_EQ(BigInt(-123) - BigInt(456), BigInt(-579));
	EXPECT_EQ(BigInt(-123) - BigInt(-456), BigInt(333));
	EXPECT_EQ(BigInt(100) - BigInt(1), BigInt(99));
	EXPECT_EQ(BigInt(0) - BigInt(0), BigInt(0));
}

TEST(BigIntTest, Multiplication) {
	EXPECT_EQ(BigInt(123) * BigInt(456), BigInt(123 * 456));
	EXPECT_EQ(BigInt(-123) * BigInt(456), BigInt(-123 * 456));
	EXPECT_EQ(BigInt(123) * BigInt(-456), BigInt(-123 * 456));
	EXPECT_EQ(BigInt(-123) * BigInt(-456), BigInt(123 * 456));
	EXPECT_EQ(BigInt(0) * BigInt(123), BigInt(0));
	EXPECT_EQ(BigInt(999) * BigInt(999), BigInt(998001));
	EXPECT_EQ(BigInt("123456789") * BigInt("987654321"),
	          BigInt("121932631112635269"));
}

TEST(BigIntTest, Division) {
	EXPECT_EQ(BigInt(456) / BigInt(123), BigInt(3));
	EXPECT_EQ(BigInt(123) / BigInt(456), BigInt(0));
	EXPECT_EQ(BigInt(-456) / BigInt(123), BigInt(-3));
	EXPECT_EQ(BigInt(456) / BigInt(-123), BigInt(-3));
	EXPECT_EQ(BigInt(1000) / BigInt(10), BigInt(100));
	EXPECT_EQ(BigInt(999) / BigInt(1000), BigInt(0));
	EXPECT_EQ(BigInt("100000000000000000000") / BigInt("10"),
	          BigInt("10000000000000000000"));

	// Деление на ноль
	EXPECT_THROW(BigInt(123) / BigInt(0), std::invalid_argument);
}

TEST(BigIntTest, CompoundAssignment) {
	BigInt a(100);
	a += BigInt(50);
	EXPECT_EQ(a, BigInt(150));

	a -= BigInt(75);
	EXPECT_EQ(a, BigInt(75));

	a *= BigInt(2);
	EXPECT_EQ(a, BigInt(150));

	a /= BigInt(3);
	EXPECT_EQ(a, BigInt(50));
}

TEST(BigIntTest, UnaryMinus) {
	EXPECT_EQ(-BigInt(123), BigInt(-123));
	EXPECT_EQ(-BigInt(-456), BigInt(456));
	EXPECT_EQ(-BigInt(0), BigInt(0));
}

TEST(BigIntTest, IncrementDecrement) {
	BigInt a(10);
	EXPECT_EQ(++a, BigInt(11));
	EXPECT_EQ(a, BigInt(11));

	EXPECT_EQ(--a, BigInt(10));
	EXPECT_EQ(a, BigInt(10));
}

// Тестирование операций сравнения
TEST(BigIntTest, ComparisonOperators) {
	EXPECT_TRUE(BigInt(123) == BigInt(123));
	EXPECT_FALSE(BigInt(123) == BigInt(456));

	EXPECT_TRUE(BigInt(123) != BigInt(456));
	EXPECT_FALSE(BigInt(123) != BigInt(123));

	EXPECT_TRUE(BigInt(123) < BigInt(456));
	EXPECT_FALSE(BigInt(456) < BigInt(123));

	EXPECT_TRUE(BigInt(123) <= BigInt(456));
	EXPECT_TRUE(BigInt(123) <= BigInt(123));

	EXPECT_TRUE(BigInt(456) > BigInt(123));
	EXPECT_FALSE(BigInt(123) > BigInt(456));

	EXPECT_TRUE(BigInt(456) >= BigInt(123));
	EXPECT_TRUE(BigInt(123) >= BigInt(123));

	EXPECT_TRUE(BigInt(-100) < BigInt(50));
	EXPECT_TRUE(BigInt(-200) < BigInt(-100));
}

// Тестирование ввода/вывода
TEST(BigIntTest, IOStreamOperators) {
	std::stringstream ss;

	// Тестирование вывода
	ss << BigInt(123);
	EXPECT_EQ(ss.str(), "123");
	ss.str("");

	ss << BigInt(-456);
	EXPECT_EQ(ss.str(), "-456");
	ss.str("");

	ss << BigInt("000123");
	EXPECT_EQ(ss.str(), "123");
	ss.str("");

	ss << BigInt("1000");
	EXPECT_EQ(ss.str(), "1000");
	ss.str("");

	// Тестирование ввода
	BigInt a;
	ss << "123456";
	ss >> a;
	EXPECT_EQ(a, BigInt(123456));
	ss.clear();

	ss << "-7890";
	ss >> a;
	EXPECT_EQ(a, BigInt(-7890));
	ss.clear();

	ss << "000100";
	ss >> a;
	EXPECT_EQ(a, BigInt(100));
}

TEST(BigIntTest, OutputZeroToStream) {
	BigInt zero(0);
	std::ostringstream oss;
	oss << zero;
	EXPECT_EQ(oss.str(), "0");
}

TEST(BigIntTest, ConstructFromZeroExplicit) {
	BigInt zero(0);
	std::stringstream ss;
	ss << zero;
	EXPECT_EQ(ss.str(), "0");
}

TEST(BigIntTest, ModOperator) {
	EXPECT_EQ(BigInt(10) % BigInt(3), BigInt(1));
	EXPECT_EQ(BigInt(-10) % BigInt(3), BigInt(-1));
	EXPECT_EQ(BigInt(10) % BigInt(-3), BigInt(1));
	EXPECT_EQ(BigInt(-10) % BigInt(-3), BigInt(-1));
	EXPECT_EQ(BigInt(100) % BigInt(7), BigInt(2));
	EXPECT_EQ(BigInt("100000000000000000000") % BigInt("9999999999"), BigInt(1));
	EXPECT_THROW(BigInt(10) % BigInt(0), std::invalid_argument);
}

TEST(BigIntTest, ModExp) {
	EXPECT_EQ(BigInt::mod_exp(BigInt(2), BigInt(10), BigInt(1000)), BigInt(24));
	EXPECT_EQ(BigInt::mod_exp(BigInt(3), BigInt(5), BigInt(7)), BigInt(5));
	EXPECT_EQ(BigInt::mod_exp(BigInt(-2), BigInt(3), BigInt(5)), BigInt(2));
	EXPECT_EQ(BigInt::mod_exp(BigInt(0), BigInt(0), BigInt(2)), BigInt(1));
	EXPECT_EQ(BigInt::mod_exp(BigInt(0), BigInt(0), BigInt(1)), BigInt(0));
	EXPECT_EQ(BigInt::mod_exp(BigInt(0), BigInt(10), BigInt(5)), BigInt(0));
	EXPECT_EQ(BigInt::mod_exp(BigInt(5), BigInt(0), BigInt(10)), BigInt(1));
	EXPECT_EQ(BigInt::mod_exp(BigInt("123456789"), BigInt("100"), BigInt("987654321")),
	          BigInt("277506981"));
	EXPECT_THROW(BigInt::mod_exp(BigInt(2), BigInt(10), BigInt(0)), std::invalid_argument);
	EXPECT_THROW(BigInt::mod_exp(BigInt(2), BigInt(-1), BigInt(5)), std::invalid_argument);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}