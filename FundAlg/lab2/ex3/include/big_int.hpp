#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <complex>
#include <cmath>

class BigInt {
private:
    unsigned long long base = 1000000;
    std::vector<unsigned long long> data;
    bool is_negative = false;

    void remove_leading_zeros();
    static bool is_correct_string(const std::string &str);

    static std::vector<std::complex<long double>> fft_transform(const std::vector<unsigned long long>& input, size_t n);
    static std::vector<unsigned long long> fft_reset(const std::vector<std::complex<long double>>& input, size_t base);
    void normalize();

    void shift_left(int k);

    static std::pair<BigInt, BigInt> divide(const BigInt & lhs, const BigInt & rhs);
public:
    BigInt();
    BigInt(const BigInt &other);
    BigInt(BigInt &&other) noexcept;
    explicit BigInt(long long l);
    explicit BigInt(const std::string &in);
    ~BigInt() = default;

    BigInt &operator=(const BigInt &other);
    BigInt &operator=(BigInt &&other) noexcept;

    [[nodiscard]] std::string to_string() const;
    void reload_from_string(const std::string & in);
    friend std::ostream &operator<<(std::ostream &out, const BigInt &num);
    friend std::istream &operator>>(std::istream &in, BigInt &num);

    void change_base(unsigned long long new_base);
    BigInt operator-();

    friend std::strong_ordering operator<=>(const BigInt & lhs, const BigInt & rhs);
    friend bool operator==(const BigInt & lhs, const BigInt & rhs);

    BigInt operator+(const BigInt & num) const;
    BigInt operator-(const BigInt & num) const;
    BigInt operator*(const BigInt & num) const;
    BigInt operator/(const BigInt & num) const;
    BigInt operator%(const BigInt & num) const;
    BigInt & operator+=(const BigInt & num);
    BigInt & operator-=(const BigInt & num);
    BigInt & operator*=(const BigInt & num);
    BigInt & operator/=(const BigInt & num);
    BigInt & operator%=(const BigInt & num);
    BigInt& operator++();
    BigInt& operator--();
    BigInt operator++(int);
    BigInt operator--(int);


    [[nodiscard]] bool is_null() const;

    static BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod);

    BigInt fft_multiply(const BigInt& a) const;

    static std::complex<long double> omega(int n, int k);
    static std::vector<std::complex<long double>> fft(size_t n, std::vector<std::complex<long double>> f, std::complex<long double> w);
};


