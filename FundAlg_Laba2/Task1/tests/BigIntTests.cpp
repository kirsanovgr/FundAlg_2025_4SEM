#include <gtest/gtest.h>
#include <sstream>
#include "../include/BigInt.h"

class BigIntTest : public ::testing::Test {};

TEST_F(BigIntTest, ConstructFromString) {
    EXPECT_EQ((std::ostringstream() << BigInt("123")).str(), "123");
    EXPECT_EQ((std::ostringstream() << BigInt("-123", 100000)).str(), "-123");
    EXPECT_EQ((std::ostringstream() << BigInt("0", 100000)).str(), "0");
}

TEST_F(BigIntTest, ConstructFromLongLong) {
    EXPECT_EQ((std::ostringstream() << BigInt(123, 100000)).str(), "123");
    EXPECT_EQ((std::ostringstream() << BigInt(-123, 100000)).str(), "-123");
    EXPECT_EQ((std::ostringstream() << BigInt(0, 100000)).str(), "0");
}

TEST_F(BigIntTest, CopyMoveConstructors) {
    BigInt a("1234567890");
    BigInt b = a;
    EXPECT_EQ((std::ostringstream() << b).str(), "1234567890");

    BigInt c = std::move(b);
    EXPECT_EQ((std::ostringstream() << c).str(), "1234567890");
}


TEST_F(BigIntTest, AssignmentOperator) {
    BigInt a("123"), b;
    b = a;
    EXPECT_EQ((std::ostringstream() << b).str(), "123");
}


TEST_F(BigIntTest, MoveAssignmentOperator) {
    BigInt a("123"), b;
    b = std::move(a);
    EXPECT_EQ((std::ostringstream() << b).str(), "123");
}


TEST_F(BigIntTest, Addition) {
    EXPECT_EQ((std::ostringstream() << (BigInt("123", 100000) + BigInt("456", 100000))).str(), "579");
    EXPECT_EQ((std::ostringstream() << (BigInt("-123", 100000) + BigInt("456", 100000))).str(), "333");
    EXPECT_EQ((std::ostringstream() << (BigInt("-123", 100000) + BigInt("-456", 100000))).str(), "-579");
}


TEST_F(BigIntTest, Subtraction) {
    EXPECT_EQ((std::ostringstream() << (BigInt("456", 100000) - BigInt("123", 100000))).str(), "333");
    EXPECT_EQ((std::ostringstream() << (BigInt("123", 100000) - BigInt("456", 100000))).str(), "-333");
    EXPECT_EQ((std::ostringstream() << (BigInt("-123", 100000) - BigInt("456", 100000))).str(), "-579");
    EXPECT_EQ((std::ostringstream() << (BigInt("-123", 100000) - BigInt("-456", 100000))).str(), "333");
}


TEST_F(BigIntTest, Multiplication) {
    EXPECT_EQ((std::ostringstream() << (BigInt("123", 100000) * BigInt("456", 100000))).str(), "56088");
    EXPECT_EQ((std::ostringstream() << (BigInt("-123", 100000) * BigInt("456", 100000))).str(), "-56088");
    EXPECT_EQ((std::ostringstream() << (BigInt("0", 100000) * BigInt("456", 100000))).str(), "0");
}


TEST_F(BigIntTest, Division) {
    EXPECT_EQ((std::ostringstream() << (BigInt("56088", 100000) / BigInt("456", 100000))).str(), "123");
    EXPECT_EQ((std::ostringstream() << (BigInt("-56088", 100000) / BigInt("456", 100000))).str(), "-123");
    EXPECT_EQ((std::ostringstream() << (BigInt("56088", 100000) / BigInt("-456", 100000))).str(), "-123");
    EXPECT_EQ((std::ostringstream() << (BigInt("-56088", 100000) / BigInt("-456", 100000))).str(), "123");
}

TEST_F(BigIntTest, Modulo) {
    EXPECT_EQ((std::ostringstream() << (BigInt("123", 100000) % BigInt("50", 100000))).str(), "23");
    EXPECT_EQ((std::ostringstream() << (BigInt("-123", 100000) % BigInt("50", 100000))).str(), "-23");
    EXPECT_EQ((std::ostringstream() << (BigInt("123", 100000) % BigInt("-50", 100000))).str(), "23");
    EXPECT_EQ((std::ostringstream() << (BigInt("-123", 100000) % BigInt("-50", 100000))).str(), "-23");
}


TEST_F(BigIntTest, IncrementDecrement) {
    BigInt a("123");
    EXPECT_EQ((std::ostringstream() << ++a).str(), "124");
    EXPECT_EQ((std::ostringstream() << a++).str(), "124");
    EXPECT_EQ((std::ostringstream() << a).str(), "125");

    EXPECT_EQ((std::ostringstream() << --a).str(), "124");
    EXPECT_EQ((std::ostringstream() << a--).str(), "124");
    EXPECT_EQ((std::ostringstream() << a).str(), "123");
}

TEST_F(BigIntTest, Comparisons) {
    EXPECT_TRUE(BigInt("123", 100000) == BigInt("123", 100000));
    EXPECT_FALSE(BigInt("123", 100000) != BigInt("123", 100000));
    EXPECT_TRUE(BigInt("123", 100000) < BigInt("456", 100000));
    EXPECT_TRUE(BigInt("-123", 100000) > BigInt("-456", 100000));
    EXPECT_TRUE(BigInt("0", 100000) == BigInt("0", 100000));
    EXPECT_TRUE(BigInt("100", 100000) <= BigInt("100", 100000));
    EXPECT_TRUE(BigInt("100", 100000) >= BigInt("100", 100000));
}


TEST_F(BigIntTest, StreamOperators) {
    std::stringstream ss;
    ss << "1234567890";
    BigInt a;
    ss >> a;
    EXPECT_EQ((std::ostringstream() << a).str(), "1234567890");

    ss.clear();
    ss.str("");
    ss << a;
    std::string result;
    ss >> result;
    EXPECT_EQ(result, "1234567890");
}


TEST_F(BigIntTest, InvalidInput) {
    EXPECT_THROW(BigInt("", 100000), std::invalid_argument);
    EXPECT_THROW(BigInt("abc", 100000), std::invalid_argument);
    EXPECT_THROW(BigInt("12-3", 100000), std::invalid_argument);
    EXPECT_THROW(BigInt("123.456", 100000), std::invalid_argument);

    std::stringstream ss;
    ss << "invalid";
    BigInt a;
    EXPECT_THROW(ss >> a, std::invalid_argument);
}


TEST_F(BigIntTest, DivisionByZero) {
    EXPECT_THROW(BigInt("123", 100000) / BigInt("0", 100000), std::domain_error);
    EXPECT_THROW(BigInt("123", 100000) % BigInt("0", 100000), std::domain_error);
}


TEST_F(BigIntTest, AdditionWithDifferentBases) {
    BigInt a("10000000000", 10);
    BigInt b("10000000", 100);
    BigInt result = a + b;
    EXPECT_EQ((std::ostringstream() << result).str(), "10010000000");
}

TEST_F(BigIntTest, SubtractionWithDifferentBases) {
    BigInt a("10000000000", 10);
    BigInt b("10000000", 100);
    BigInt result = a - b;
    EXPECT_EQ((std::ostringstream() << result).str(), "9990000000");
}


TEST_F(BigIntTest, MultiplicationWithDifferentBases) {
    BigInt a("100000", 10);
    BigInt b("1000", 100);
    BigInt result = a * b;
    EXPECT_EQ((std::ostringstream() << result).str(), "100000000");
}

TEST_F(BigIntTest, DivisionWithDifferentBases) {
    BigInt a("100000000", 10);
    BigInt b("1000", 100);
    BigInt result = a / b;
    EXPECT_EQ((std::ostringstream() << result).str(), "100000");
}

TEST_F(BigIntTest, UnaryMinus) {
    BigInt a("123");
    BigInt b = -a;
    EXPECT_EQ((std::ostringstream() << b).str(), "-123");

    BigInt c = -b;
    EXPECT_EQ((std::ostringstream() << c).str(), "123");
}

TEST_F(BigIntTest, MultiplicationWithIntegers) {
    BigInt a("123");
    BigInt result1 = a * 456LL;
    EXPECT_EQ((std::ostringstream() << result1).str(), "56088");

    BigInt result2 = a * 456ULL;
    EXPECT_EQ((std::ostringstream() << result2).str(), "56088");
}


TEST_F(BigIntTest, CombinedAssignmentOperations) {
    BigInt a("100");
    a += BigInt("50");
    EXPECT_EQ((std::ostringstream() << a).str(), "150");

    a -= BigInt("25");
    EXPECT_EQ((std::ostringstream() << a).str(), "125");

    a *= BigInt("2");
    EXPECT_EQ((std::ostringstream() << a).str(), "250");

    a /= BigInt("10");
    EXPECT_EQ((std::ostringstream() << a).str(), "25");

    a %= BigInt("7");
    EXPECT_EQ((std::ostringstream() << a).str(), "4");
}


TEST_F(BigIntTest, ZeroOperations) {
    BigInt zero("0");
    BigInt num("12345");

    EXPECT_EQ(num + zero, num);
    EXPECT_EQ(num - zero, num);
    EXPECT_EQ(num * zero, zero);
    EXPECT_EQ(zero / num, zero);
    EXPECT_EQ(zero % num, zero);
}


TEST_F(BigIntTest, OneOperations) {
    BigInt zero(0);
    BigInt one("1");
    BigInt num("12345");

    EXPECT_EQ(num * one, num);
    EXPECT_EQ(num / num, one);
    EXPECT_EQ(num % one, zero);
    EXPECT_EQ(one * -1LL, BigInt(-1));
    EXPECT_EQ(++BigInt("0"), one);
}

TEST_F(BigIntTest, IncrementDecrementThroughZero) {
    BigInt a("-1");
    EXPECT_EQ(++a, BigInt("0"));
    EXPECT_EQ(++a, BigInt("1"));

    BigInt b("1");
    EXPECT_EQ(--b, BigInt("0"));
    EXPECT_EQ(--b, BigInt("-1"));
}

TEST_F(BigIntTest, VeryLargeNumbers) {
    std::string bigStr1(50, '9');
    std::string bigStr2(50, '9');

    BigInt a(bigStr1);
    BigInt b(bigStr2);

    BigInt sum = a + b;
    std::string expectedSum = "1" + std::string(50, '9');
    expectedSum[expectedSum.size()-1] = '8';
    EXPECT_EQ((std::ostringstream() << sum).str(), expectedSum);

    BigInt product = a * b;
    std::string expectedProduct = std::string(49, '9') + "8" + std::string(49, '0') + "1";
    EXPECT_EQ((std::ostringstream() << product).str(), expectedProduct);
}

TEST_F(BigIntTest, DivisionWithRemainder) {
    BigInt a("123456");
    BigInt b("789");
    BigInt quotient = a / b;
    BigInt remainder = a % b;
    EXPECT_EQ((std::ostringstream() << quotient).str(), "156");
    EXPECT_EQ((std::ostringstream() << remainder).str(), "372");
}

TEST_F(BigIntTest, OperationExceptions) {
    EXPECT_THROW(BigInt("123") / BigInt("0"), std::domain_error);
    EXPECT_THROW(BigInt("123") % BigInt("0"), std::domain_error);

    EXPECT_THROW(BigInt("12a34"), std::invalid_argument);
    EXPECT_THROW(BigInt("--123"), std::invalid_argument);
}

TEST_F(BigIntTest, ComparisonWithDifferentBases) {
    BigInt a("10000000000", 10);
    BigInt b("10000000000", 100);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a > -b);
    EXPECT_FALSE(-a > b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a > b);

    BigInt c("9999999999", 10);
    EXPECT_TRUE(c < a);
    EXPECT_TRUE(a > c);
}

TEST_F(BigIntTest, LeadingZerosInString) {
    BigInt a("0000012345");
    EXPECT_EQ((std::ostringstream() << a).str(), "12345");

    BigInt b("-000000123");
    EXPECT_EQ((std::ostringstream() << b).str(), "-123");
}

TEST_F(BigIntTest, InputWithLeadingZeros) {
    std::stringstream ss("00000000123456789");
    BigInt a;
    ss >> a;
    EXPECT_EQ((std::ostringstream() << a).str(), "123456789");
}

TEST_F(BigIntTest, IncorrectBase) {
    EXPECT_THROW(BigInt("1234", 13), std::invalid_argument);
    EXPECT_THROW(BigInt(100,  15), std::invalid_argument);
    EXPECT_THROW(BigInt(101,  0), std::invalid_argument);
}
