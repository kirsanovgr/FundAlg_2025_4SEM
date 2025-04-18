#include "../include/bigint.hpp"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

class BigIntTest : public ::testing::Test {
   protected:
	BigInt zero;
	BigInt one;
	BigInt ten;
	BigInt neg_one;
	BigInt pos_small;
	BigInt neg_small;
	BigInt pos_large;
	BigInt neg_large;
	BigInt base_val;
	BigInt near_base;
	BigInt base_times_two;

	void SetUp() override {
		zero = BigInt(0);
		one = BigInt(1);
		ten = BigInt(10);
		neg_one = BigInt(-1);
		pos_small = BigInt(123);
		neg_small = BigInt("-123");
		pos_large = BigInt("1000000005");
		neg_large = BigInt("-1000000005");
		base_val = BigInt("1000000000");
		near_base = BigInt("999999999");
		base_times_two = BigInt("2000000000");
	}
};

TEST_F(BigIntTest, Constructors) {
	BigInt default_constructed;
	EXPECT_EQ(default_constructed, zero);
	EXPECT_EQ(BigInt(123), pos_small);
	EXPECT_EQ(BigInt(-123), neg_small);
	EXPECT_EQ(BigInt(0), zero);
	EXPECT_EQ(BigInt(1000000000), base_val);
	EXPECT_EQ(BigInt("0"), zero);
	EXPECT_EQ(BigInt("+123"), pos_small);
	EXPECT_EQ(BigInt("-000123"), neg_small);
	EXPECT_EQ(BigInt("1000000005"), pos_large);
	EXPECT_EQ(BigInt("2000000000"), base_times_two);
	BigInt copy_ctor_test = pos_large;
	EXPECT_EQ(copy_ctor_test, pos_large);
	BigInt move_ctor_src("98765");
	BigInt move_ctor_dest = std::move(move_ctor_src);
	EXPECT_EQ(move_ctor_dest, BigInt("98765"));
	EXPECT_EQ(move_ctor_src, zero);
	EXPECT_THROW(BigInt(""), std::invalid_argument);
	EXPECT_THROW(BigInt("-"), std::invalid_argument);
	EXPECT_THROW(BigInt("+"), std::invalid_argument);
	EXPECT_THROW(BigInt("abc"), std::invalid_argument);
	EXPECT_THROW(BigInt("123a4"), std::invalid_argument);
	EXPECT_THROW(BigInt("--1"), std::invalid_argument);
}

TEST_F(BigIntTest, Assignment) {
	BigInt assigned_val;
	assigned_val = pos_small;
	EXPECT_EQ(assigned_val, pos_small);
	assigned_val = neg_large;
	EXPECT_EQ(assigned_val, neg_large);
	assigned_val = assigned_val;
	EXPECT_EQ(assigned_val, neg_large);

	BigInt move_assign_src("54321");
	BigInt move_assign_dest;
	move_assign_dest = std::move(move_assign_src);
	EXPECT_EQ(move_assign_dest, BigInt("54321"));
	EXPECT_EQ(move_assign_src, zero);
	move_assign_dest = std::move(move_assign_dest);
	EXPECT_EQ(move_assign_dest, BigInt("54321"));
}

TEST_F(BigIntTest, Comparison) {
	EXPECT_EQ(pos_small, BigInt(123));
	EXPECT_NE(pos_small, neg_small);
	EXPECT_NE(pos_large, zero);
	EXPECT_EQ(zero, BigInt("0"));
	EXPECT_EQ(BigInt("-0"), zero);

	EXPECT_LT(neg_small, zero);
	EXPECT_LT(neg_large, neg_small);
	EXPECT_LT(zero, pos_small);
	EXPECT_LT(pos_small, pos_large);
	EXPECT_LT(near_base, base_val);

	EXPECT_GT(zero, neg_large);
	EXPECT_GT(pos_small, zero);
	EXPECT_GT(pos_large, pos_small);
	EXPECT_GT(base_val, near_base);
	EXPECT_GT(pos_small, neg_small);

	EXPECT_LE(neg_small, neg_small);
	EXPECT_LE(neg_small, zero);
	EXPECT_LE(zero, zero);
	EXPECT_LE(zero, pos_small);
	EXPECT_LE(pos_small, pos_small);
	EXPECT_LE(pos_small, pos_large);

	EXPECT_GE(pos_large, pos_large);
	EXPECT_GE(pos_large, pos_small);
	EXPECT_GE(pos_small, zero);
	EXPECT_GE(zero, zero);
	EXPECT_GE(zero, neg_small);
	EXPECT_GE(neg_small, neg_small);
	EXPECT_GE(neg_small, neg_large);
}

TEST_F(BigIntTest, AdditionAssignment) {
	BigInt a = pos_small;
	a += one;
	EXPECT_EQ(a, BigInt(124));
	a = pos_small;
	a += zero;
	EXPECT_EQ(a, pos_small);
	a = zero;
	a += pos_small;
	EXPECT_EQ(a, pos_small);
	a = pos_small;
	a += neg_small;
	EXPECT_EQ(a, zero);
	a = neg_small;
	a += pos_small;
	EXPECT_EQ(a, zero);
	a = neg_small;
	a += neg_one;
	EXPECT_EQ(a, BigInt(-124));
	a = near_base;
	a += one;
	EXPECT_EQ(a, base_val);
	a = pos_large;
	a += neg_small;
	EXPECT_EQ(a, BigInt("999999882"));
	a = neg_large;
	a += pos_small;
	EXPECT_EQ(a, BigInt("-999999882"));
	a = neg_large;
	a += neg_small;
	EXPECT_EQ(a, BigInt("-1000000128"));
	a = pos_large;
	a += pos_large;
	EXPECT_EQ(a, BigInt("2000000010"));
}

TEST_F(BigIntTest, SubtractionAssignment) {
	BigInt a = pos_small;
	a -= one;
	EXPECT_EQ(a, BigInt(122));
	a = pos_small;
	a -= zero;
	EXPECT_EQ(a, pos_small);
	a = zero;
	a -= pos_small;
	EXPECT_EQ(a, neg_small);
	a = pos_small;
	a -= neg_small;
	EXPECT_EQ(a, BigInt(246));
	a = neg_small;
	a -= pos_small;
	EXPECT_EQ(a, BigInt(-246));
	a = neg_small;
	a -= neg_one;
	EXPECT_EQ(a, BigInt(-122));
	a = base_val;
	a -= one;
	EXPECT_EQ(a, near_base);
	a = pos_large;
	a -= pos_small;
	EXPECT_EQ(a, BigInt("999999882"));
	a = neg_large;
	a -= neg_small;
	EXPECT_EQ(a, BigInt("-999999882"));
	a = pos_small;
	a -= pos_large;
	EXPECT_EQ(a, BigInt("-999999882"));
	a = zero;
	a -= zero;
	EXPECT_EQ(a, zero);
}

TEST_F(BigIntTest, MultiplicationAssignment) {
	BigInt a = pos_small;
	a *= ten;
	EXPECT_EQ(a, BigInt(1230));
	a = pos_small;
	a *= zero;
	EXPECT_EQ(a, zero);
	a = zero;
	a *= pos_small;
	EXPECT_EQ(a, zero);
	a = pos_small;
	a *= neg_one;
	EXPECT_EQ(a, neg_small);
	a = neg_small;
	a *= neg_one;
	EXPECT_EQ(a, pos_small);
	a = neg_small;
	a *= BigInt(-2);
	EXPECT_EQ(a, BigInt(246));
	a = near_base;
	a *= BigInt(2);
	EXPECT_EQ(a, BigInt("1999999998"));
	a = base_val;
	a *= base_val;
	EXPECT_EQ(a, BigInt("1000000000000000000"));
	a = one;
	a *= pos_large;
	EXPECT_EQ(a, pos_large);
	a = neg_one;
	a *= pos_large;
	EXPECT_EQ(a, neg_large);
}

TEST_F(BigIntTest, DivisionAssignment) {
	BigInt a = BigInt(1230);
	a /= ten;
	EXPECT_EQ(a, pos_small);
	a = pos_small;
	a /= one;
	EXPECT_EQ(a, pos_small);
	a = pos_small;
	a /= neg_one;
	EXPECT_EQ(a, neg_small);
	a = neg_small;
	a /= one;
	EXPECT_EQ(a, neg_small);
	a = neg_small;
	a /= neg_one;
	EXPECT_EQ(a, pos_small);
	a = BigInt("1999999998");
	a /= BigInt(2);
	EXPECT_EQ(a, near_base);
	a = BigInt("1000000000000000000");
	a /= base_val;
	EXPECT_EQ(a, base_val);
	a = pos_large;
	a /= pos_small;
	EXPECT_EQ(a, BigInt("8130081"));
	a = pos_large;
	a /= one;
	EXPECT_EQ(a, pos_large);
	a = pos_large;
	a /= pos_large;
	EXPECT_EQ(a, one);
	a = pos_small;
	a /= pos_large;
	EXPECT_EQ(a, zero);
	a = zero;
	a /= pos_small;
	EXPECT_EQ(a, zero);
	EXPECT_THROW(pos_small /= zero, std::runtime_error);
	EXPECT_THROW(zero /= zero, std::runtime_error);
}

TEST_F(BigIntTest, BinaryArithmetic) {
	EXPECT_EQ(pos_small + neg_small, zero);
	EXPECT_EQ(base_val + near_base, BigInt("1999999999"));
	EXPECT_EQ(pos_small - pos_small, zero);
	EXPECT_EQ(base_val - near_base, one);
	EXPECT_EQ(pos_small * ten, BigInt(1230));
	EXPECT_EQ(pos_small * zero, zero);
	EXPECT_EQ(pos_large / pos_large, one);
	EXPECT_EQ(pos_small / ten, BigInt(12));
	EXPECT_EQ(neg_small / ten, BigInt(-12));
	EXPECT_THROW(one / zero, std::runtime_error);
}

TEST_F(BigIntTest, IncrementDecrement) {
	BigInt a = pos_small;
	EXPECT_EQ(++a, BigInt(124));
	EXPECT_EQ(a, BigInt(124));
	a = pos_small;
	EXPECT_EQ(a++, pos_small);
	EXPECT_EQ(a, BigInt(124));
	a = zero;
	EXPECT_EQ(++a, one);
	EXPECT_EQ(a, one);
	a = zero;
	EXPECT_EQ(a++, zero);
	EXPECT_EQ(a, one);
	a = one;
	EXPECT_EQ(--a, zero);
	EXPECT_EQ(a, zero);
	a = one;
	EXPECT_EQ(a--, one);
	EXPECT_EQ(a, zero);
	a = neg_one;
	EXPECT_EQ(++a, zero);
	EXPECT_EQ(a, zero);
	a = neg_one;
	EXPECT_EQ(a++, neg_one);
	EXPECT_EQ(a, zero);
	a = zero;
	EXPECT_EQ(--a, neg_one);
	EXPECT_EQ(a, neg_one);
	a = zero;
	EXPECT_EQ(a--, zero);
	EXPECT_EQ(a, neg_one);
}

TEST_F(BigIntTest, StreamOutput) {
	std::stringstream ss;
	ss << zero;
	EXPECT_EQ(ss.str(), "0");
	ss.str("");
	ss.clear();
	ss << pos_small;
	EXPECT_EQ(ss.str(), "123");
	ss.str("");
	ss.clear();
	ss << neg_small;
	EXPECT_EQ(ss.str(), "-123");
	ss.str("");
	ss.clear();
	ss << base_val;
	EXPECT_EQ(ss.str(), "1000000000");
	ss.str("");
	ss.clear();
	ss << pos_large;
	EXPECT_EQ(ss.str(), "1000000005");
	ss.str("");
	ss.clear();
	BigInt large("1234567890123456789");
	ss << large;
	EXPECT_EQ(ss.str(), "1234567890123456789");
}

TEST_F(BigIntTest, StreamInput) {
	std::stringstream ss;
	BigInt val;
	ss << "123 -456 0 1000000000000000000 invalid +987";
	ss >> val;
	EXPECT_EQ(val, BigInt(123));
	EXPECT_FALSE(ss.fail());
	ss >> val;
	EXPECT_EQ(val, BigInt(-456));
	EXPECT_FALSE(ss.fail());
	ss >> val;
	EXPECT_EQ(val, zero);
	EXPECT_FALSE(ss.fail());
	ss >> val;
	EXPECT_EQ(val, BigInt("1000000000000000000"));
	EXPECT_FALSE(ss.fail());
	val = BigInt(111);
	ss >> val;
	EXPECT_TRUE(ss.fail());
	EXPECT_EQ(val, BigInt(111));
	ss.clear();
	ss >> val;
	EXPECT_EQ(val, BigInt(987));
	EXPECT_FALSE(ss.fail());

	ss.str("");
	ss.clear();
	ss << "";
	ss >> val;
	EXPECT_TRUE(ss.fail());
	ss.str("");
	ss.clear();
	ss << "-";
	ss >> val;
	EXPECT_TRUE(ss.fail());
}
TEST_F(BigIntTest, ModuloAndExponentiation) {
    
    BigInt a = pos_small; 
    BigInt b = ten;     
    EXPECT_EQ(a % b, BigInt(3));
    a %= b;
    EXPECT_EQ(a, BigInt(3));

    a = neg_small; 
    EXPECT_EQ(a % b, BigInt("-3")); 
    a %= b;
    EXPECT_EQ(a, BigInt("-3"));

    a = pos_small; 
    b = BigInt(-10);
    EXPECT_EQ(a % b, BigInt("3"));
    a %= b;
    EXPECT_EQ(a, BigInt("3"));

    a = neg_small; 
    EXPECT_EQ(a % b, BigInt("-3"));
    a %= b;
    EXPECT_EQ(a, BigInt("-3"));

    a = BigInt(120);
    EXPECT_EQ(a % ten, zero);
    a %= ten;
    EXPECT_EQ(a, zero);

    EXPECT_THROW(a %= zero, std::runtime_error);
    EXPECT_THROW(a = pos_small % zero, std::runtime_error);

    BigInt mod_val(100);
    EXPECT_EQ(BigInt::mod_exp(BigInt(3), BigInt(4), mod_val), BigInt(81)); 
    EXPECT_EQ(BigInt::mod_exp(BigInt(2), BigInt(10), BigInt(1000)), BigInt(24));
    EXPECT_EQ(BigInt::mod_exp(pos_small, zero, mod_val), one); 
    EXPECT_EQ(BigInt::mod_exp(neg_small, BigInt(3), mod_val), BigInt(33));
    EXPECT_EQ(BigInt::mod_exp(pos_large, ten, base_val), BigInt("9765625"));


    EXPECT_THROW(BigInt::mod_exp(ten, ten, zero), std::runtime_error); 
    EXPECT_THROW(BigInt::mod_exp(ten, neg_one, ten), std::invalid_argument); 
    EXPECT_EQ(BigInt::mod_exp(ten, ten, one), zero);
	EXPECT_EQ(BigInt::mod_exp(0, BigInt(4), mod_val), BigInt(0));
}