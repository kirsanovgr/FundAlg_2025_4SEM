#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <cmath>
#include <sstream>

class BigInt {
public:
    BigInt();
    explicit BigInt(long long value, unsigned int base = 10);
    explicit BigInt(const std::string& str, unsigned int base = 10);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    ~BigInt() = default;

    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;

    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);
    BigInt& operator%=(const BigInt& other);

    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);

    bool operator==(const BigInt& other) const;
    std::strong_ordering operator<=>(const BigInt& other) const;

    [[nodiscard]] unsigned int getBase() const { return base; }

    friend std::istream& operator>>(std::istream& is, BigInt& num);
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

    static BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod);

private:
    std::vector<unsigned long long> digits;
    bool isNegative;
    unsigned int base;

    [[nodiscard]] bool is_empty() const { return (digits.size() == 1 && digits[0] == 0) || digits.empty();}

    void removeLeadingZeros();
    [[nodiscard]] int compareMagnitude(const BigInt& other) const;

    [[nodiscard]] BigInt absoluteAdd(const BigInt& other) const;
    [[nodiscard]] BigInt absoluteSubtract(const BigInt& other) const;

    [[nodiscard]] std::pair<BigInt, BigInt> divide(const BigInt& divisor) const;

    static BigInt convertToBase(const BigInt& bigInt, unsigned int newBase);
};


BigInt::BigInt() : isNegative(false), base(10) {
    digits.push_back(0);
}

BigInt::BigInt(long long value, unsigned int base_) : isNegative(false), base(base_) {
    if (base % 10 != 0) {
        throw std::invalid_argument("Base must be power of 10");
    }

    if (value < 0) {
        isNegative = true;
        value = -value;
    }

    unsigned long long abs_value = value;
    if (abs_value == 0) {
        digits.push_back(0);
    } else {
        while (abs_value > 0) {
            digits.push_back(abs_value % base);
            abs_value /= base;
        }
    }

    if (digits.empty()) {
        digits.push_back(0);
        isNegative = false;
    }
}

BigInt::BigInt(const std::string& str, unsigned int base_) : isNegative(false), base(base_) {
    if (base_ % 10 != 0) {
        throw std::invalid_argument("Base must be a power of 10");
    }

    if (base_ <= 1) {
        throw std::invalid_argument("Base must be greater than 1");
    }

    size_t start = 0;
    if (!str.empty() && str[0] == '-') {
        isNegative = true;
        start = 1;
    }

    std::string num_str = str.substr(start);
    digits.clear();

    if (num_str.empty()) {
        digits.push_back(0);
        isNegative = false;
        return;
    }

    size_t chunk_size = static_cast<size_t>(std::log10(base_));

    if (chunk_size == 0) {
        throw std::invalid_argument("Chunk size must be greater than zero");
    }

    size_t total_digits = num_str.length();
    size_t padding = (chunk_size - (total_digits % chunk_size)) % chunk_size;
    num_str = std::string(padding, '0') + num_str;

    for (size_t pos = num_str.length(); pos > 0; ) {
        size_t start_pos = (pos > chunk_size) ? pos - chunk_size : 0;

        size_t chunk_length = (pos - start_pos);

        std::string chunk = num_str.substr(start_pos, chunk_length);
        unsigned long long digit = std::stoull(chunk);
        digits.push_back(digit);

        if (chunk_size == 0) break;
        pos -= chunk_size;
    }

    removeLeadingZeros();
}

BigInt::BigInt(const BigInt& other) = default;

BigInt::BigInt(BigInt&& other) noexcept
        : digits(std::move(other.digits)),
          isNegative(other.isNegative),
          base(other.base) {}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        digits = other.digits;
        isNegative = other.isNegative;
        base = other.base;
    }
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        digits = std::move(other.digits);
        isNegative = other.isNegative;
        base = other.base;
    }
    return *this;
}

void BigInt::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
        digits.clear();
        digits.push_back(0);
        isNegative = false;
    }
}

int BigInt::compareMagnitude(const BigInt& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size() ? 1 : -1;
    }

    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] > other.digits[i] ? 1 : -1;
        }
    }
    return 0;
}

BigInt BigInt::absoluteAdd(const BigInt& other) const {
    BigInt result(0, base);
    size_t maxSize = std::max(digits.size(), other.digits.size());
    unsigned long long carry = 0;
    result.digits.clear();

    for (size_t i = 0; i < maxSize || carry; ++i) {
        unsigned long long sum = carry;
        if (i < digits.size()) sum += digits[i];
        if (i < other.digits.size()) sum += other.digits[i];

        carry = sum / base;
        result.digits.push_back(sum % base);
    }

    return result;
}

BigInt BigInt::absoluteSubtract(const BigInt& other) const {
    BigInt result(0, base);
    unsigned long long borrow = 0;
    result.digits.clear();

    for (size_t i = 0; i < digits.size(); ++i) {
        long long diff = (long long)digits[i] - (long long)borrow;
        borrow = 0;

        if (i < other.digits.size()) {
            diff -= other.digits[i];
        }

        if (diff < 0) {
            diff += base;
            borrow = 1;
        }

        result.digits.push_back(diff);
    }

    result.removeLeadingZeros();
    return result;
}

std::pair<BigInt, BigInt> BigInt::divide(const BigInt& divisor) const {
    if (divisor == BigInt(0, base)) {
        throw std::invalid_argument("Division by zero");
    }

    BigInt quotient(0, base);
    BigInt remainder = *this;
    remainder.isNegative = false;
    BigInt absDivisor = divisor;
    absDivisor.isNegative = false;

    if (remainder < absDivisor) {
        return {BigInt(0, base), remainder};
    }

    BigInt current(0, base);
    quotient.digits.resize(digits.size(), 0);

    for (int i = digits.size() - 1; i >= 0; --i) {
        current = current * BigInt(base, base) + BigInt((long long)digits[i], base);
        unsigned long long l = 0, r = base;

        while (l < r) {
            unsigned long long m = (l + r + 1) / 2;
            BigInt temp = absDivisor * BigInt(m, base);
            if (temp <= current) {
                l = m;
            } else {
                r = m - 1;
            }
        }

        quotient.digits[i] = l;
        current = current - absDivisor * BigInt(l, base);
    }

    quotient.isNegative = isNegative != divisor.isNegative;
    quotient.removeLeadingZeros();
    current.isNegative = isNegative;
    return {quotient, current};
}

BigInt BigInt::convertToBase(const BigInt& bigInt, unsigned int newBase) {
    if (newBase == bigInt.base) {
        return bigInt;
    }

    BigInt result(0, newBase);
    BigInt power(1, newBase);

    for (unsigned long long digit : bigInt.digits) {
        BigInt term = BigInt(digit, newBase) * power;
        result += term;
        power = power * BigInt(bigInt.base, newBase);
    }

    result.isNegative = bigInt.isNegative;
    return result;
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = convertToBase(*this, 10);
        BigInt b = convertToBase(other, 10);
        return a + b;
    }

    BigInt result(0, base);
    if (isNegative == other.isNegative) {
        result = absoluteAdd(other);
        result.isNegative = isNegative;
    } else {
        int cmp = compareMagnitude(other);
        if (cmp == 0) {
            return BigInt(0, base);
        } else if (cmp > 0) {
            result = absoluteSubtract(other);
            result.isNegative = isNegative;
        } else {
            result = other.absoluteSubtract(*this);
            result.isNegative = other.isNegative;
        }
    }

    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = convertToBase(*this, 10);
        BigInt b = convertToBase(other, 10);
        return a - b;
    }

    BigInt temp = other;
    temp.isNegative = !temp.isNegative;
    return *this + temp;
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = convertToBase(*this, 10);
        BigInt b = convertToBase(other, 10);
        return a * b;
    }

    BigInt result(0, base);
    result.digits.resize(digits.size() + other.digits.size(), 0);
    result.isNegative = isNegative != other.isNegative;

    for (size_t i = 0; i < digits.size(); ++i) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            size_t idx = i + j;
            unsigned long long product = digits[i] * (j < other.digits.size() ? other.digits[j] : 0);
            unsigned long long total = result.digits[idx] + product + carry;
            carry = total / base;
            result.digits[idx] = total % base;
        }
    }

    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = convertToBase(*this, 10);
        BigInt b = convertToBase(other, 10);
        return a / b;
    }

    auto [quotient, remainder] = divide(other);
    return quotient;
}

BigInt BigInt::operator%(const BigInt& other) const {
    if (other == BigInt(0, base)) {
        throw std::invalid_argument("Modulo by zero");
    }

    if (base != other.base) {
        BigInt a = convertToBase(*this, 10);
        BigInt b = convertToBase(other, 10);
        return a % b;
    }

    BigInt remainder = divide(other).second;
    remainder.isNegative = this->isNegative;
    return remainder;
}

BigInt& BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& other) {
    *this = *this / other;
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& other) {
    *this = *this % other;
    return *this;
}

BigInt& BigInt::operator++() {
    *this += BigInt(1, base);
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    ++*this;
    return temp;
}

BigInt& BigInt::operator--() {
    *this -= BigInt(1, base);
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --*this;
    return temp;
}

bool BigInt::operator==(const BigInt& other) const {
    if (base != other.base) {
        BigInt b = convertToBase(other, this->base);
        return *this == b;
    }
    return isNegative == other.isNegative && digits == other.digits;
}

std::strong_ordering BigInt::operator<=>(const BigInt& other) const {
    if (isNegative != other.isNegative) {
        return isNegative ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size() ? std::strong_ordering::greater : std::strong_ordering::less;
    }

    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] > other.digits[i] ? std::strong_ordering::greater : std::strong_ordering::less;
        }
    }

    return std::strong_ordering::equal;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    std::stringstream output;
    if (num.isNegative) {
        output << "-";
    }

    for (auto it = num.digits.rbegin(); it != num.digits.rend(); ++it) {
        output << *it;
    }

    os << output.str();
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string str;
    is >> str;
    num = BigInt(str, num.getBase());
    return is;
}

BigInt BigInt::mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod) {
    if (exp.is_empty()) {
        return BigInt(1, exp.base);
    }

    if (mod == BigInt(1)) {
        return BigInt(0);
    }

    BigInt result(1, base.base);
    BigInt current_base = base % mod;
    BigInt current_exp = exp;
    BigInt two(2);

    while (current_exp > BigInt(0)) {
        if ((current_exp.digits[0] % 2) != 0) {
            result = (result * current_base) % mod;
        }
        current_base = (current_base * current_base) % mod;
        current_exp = current_exp / two;
    }

    return result;
}