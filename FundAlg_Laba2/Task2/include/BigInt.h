#ifndef TASK1_BIGINT_H
#define TASK1_BIGINT_H


#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <compare>

class BigInt {

    static bool is_correct_string(const std::string &str);

    void string_in_digits(const std::string &in);

    [[nodiscard]] std::string digits_in_string() const;

    void remove_leading_zeros();

    void change_base(unsigned long long int new_base);

    void unsign_sum(const BigInt &a1);

    void unsign_sum(unsigned long long num);

    void unsign_sub(const BigInt &a1);

    [[nodiscard]] BigInt abs_num() const;

    [[nodiscard]] BigInt mul_size_t(unsigned long long num) const;

    [[nodiscard]] std::pair<BigInt, BigInt> del_num(const BigInt &other) const;

    static bool base_valid(unsigned long long test_base);


public:
    explicit BigInt();

    explicit BigInt(long long int value, unsigned long long bs = 100000);

    explicit BigInt(const std::string &str, unsigned long long bs = 100000);

    BigInt(const BigInt &other) = default;

    BigInt(BigInt &&other) noexcept;

    ~BigInt() = default;

    BigInt &operator=(const BigInt &other);

    BigInt &operator=(BigInt &&other) noexcept;

    BigInt operator+(const BigInt &other) const;

    BigInt operator-(const BigInt &other) const;

    BigInt operator-() const;

    BigInt operator*(const BigInt &other) const;

    BigInt operator*(const long long num) const;

    BigInt operator*(const unsigned long long num) const;

    BigInt operator/(const BigInt &other) const;

    BigInt operator%(const BigInt &other) const;

    BigInt operator+=(const BigInt &other);

    BigInt operator-=(const BigInt &other);

    BigInt operator*=(const BigInt &other);

    BigInt operator/=(const BigInt &other);

    BigInt operator%=(const BigInt &other);

    BigInt operator++();

    BigInt operator++(int);


    BigInt operator--();

    BigInt operator--(int);

    std::strong_ordering operator<=>(const BigInt &other) const;

    bool operator==(const BigInt &other) const;

    bool operator!=(const BigInt &other) const;

    friend std::istream &operator>>(std::istream &is, BigInt &num);

    friend std::ostream &operator<<(std::ostream &os, const BigInt &num);

private:
    std::vector<unsigned long long> digits;
    bool isNegative;
    unsigned long long base_osnov = 100000;
};

BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod);


#endif //TASK1_BIGINT_H
