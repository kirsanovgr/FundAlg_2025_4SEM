#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

class BigInt {
public:
    BigInt();
    explicit BigInt(long long value);
    explicit BigInt(const std::string& str);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    // сам удалит vector
    ~BigInt() = default;

    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;

    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);
    BigInt& operator++();
    BigInt& operator--();

    std::string to_string() const;

    std::strong_ordering operator<=>(const BigInt &rhs) const;
    bool operator==(const BigInt &rhs) const;

    friend std::istream& operator>>(std::istream& is, BigInt& num);
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

private:
    // максимальный размер числа в одной ячейке digits
    static const unsigned long long base = 100000;
    std::vector<unsigned long long> digits;
    bool isNegative;
};

// BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod) const;
// BigInt fft_multiply(const BigInt& a) const;
// BigInt karatsuba_multiply(const BigInt& a) const;
// BigInt newton_divide(const BigInt& a) const;
