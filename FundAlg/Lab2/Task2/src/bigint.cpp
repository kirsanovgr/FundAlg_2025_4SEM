#include "../include/bigint.hpp"
#include <cmath>
#include <iomanip>
#include <ranges>
#include <sstream>
#include <stdexcept>

void BigInt::parse_unsigned_value(unsigned long long value) {
    while (value > 0) {
        digits.push_back(value % base);
        value /= base;
    }
}

void BigInt::remove_leading_zeros() {
    while (!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty())
        isNegative = false;
}

std::strong_ordering BigInt::compare_absolutes(const BigInt& a, const BigInt& b) {
    if (a.digits.size() != b.digits.size()) {
        return a.digits.size() <=> b.digits.size();
    }
    for (auto it_a = a.digits.rbegin(), it_b = b.digits.rbegin();
         it_a != a.digits.rend();
         ++it_a, ++it_b) {
        if (*it_a != *it_b) {
            return *it_a <=> *it_b;
        }
    }
    return std::strong_ordering::equal;
}

BigInt BigInt::subtract_unsigned(const BigInt& a, const BigInt& b) const {
    BigInt result;
    result.base = a.base;
    unsigned long long borrow = 0;
    for (size_t i = 0; i < a.digits.size(); ++i) {
        long long current = a.digits[i] - borrow;
        borrow = 0;
        if (i < b.digits.size()) {
            current -= b.digits[i];
        }
        if (current < 0) {
            current += base;
            borrow = 1;
        }
        result.digits.push_back(static_cast<unsigned long long>(current));
    }
    result.remove_leading_zeros();
    return result;
}

BigInt::BigInt(long long value) {
    if (value < 0) {
        value *= -1;
        isNegative = true;
    } else if (value == 0) {
        return;
    }
    parse_unsigned_value(value);
}

BigInt::BigInt(const std::string& str) {
    size_t index = 0;
    if (str[index] == '-') {
        index++;
    	if (!str[index]) {
    		throw std::invalid_argument("String is incorrect");
    	}
        isNegative = true;
    }
    size_t temp = 0;
    size_t multiplier = 1;
    int temp_size = 0;
    int block_size = static_cast<int>(log10((double)base));
    for (size_t end = str.length() - 1; end > index; end--) {
        if (!isdigit(str[end])) {
	        throw std::invalid_argument("String contains non-digit characters");
        }
        temp += (str[end] - '0') * multiplier;
        multiplier *= 10;
        temp_size++;
        if (temp_size == block_size) {
            digits.push_back(temp);
            temp = 0;
            temp_size = 0;
            multiplier = 1;
        }
    }
    temp += (str[index] - '0') * multiplier;
    if (temp > 0)
        digits.push_back(temp);
    if (temp == 0 && temp_size == 0)
        isNegative = false;
}

BigInt::BigInt(const BigInt& other) {
    digits = other.digits;
    base = other.base;
    isNegative = other.isNegative;
}

BigInt::BigInt(BigInt&& other) noexcept {
    digits = std::move(other.digits);
    base = other.base;
    isNegative = other.isNegative;
	other.isNegative = false;
}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this == &other)
        return *this;
    digits = other.digits;
    base = other.base;
    isNegative = other.isNegative;
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this == &other)
        return *this;
    digits = std::move(other.digits);
    base = other.base;
    isNegative = other.isNegative;
	other.isNegative = false;
    return *this;
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (isNegative == other.isNegative) {
        BigInt result;
        result.isNegative = isNegative;
        result.base = base;
        size_t carry = 0;
        size_t max_length = std::max(digits.size(), other.digits.size());
        for (size_t i = 0; i < max_length || carry; ++i) {
            size_t current_sum = carry;
            if (i < digits.size())
                current_sum += digits[i];
            if (i < other.digits.size())
                current_sum += other.digits[i];
            result.digits.push_back(current_sum % base);
            carry = current_sum / base;
        }
        result.remove_leading_zeros();
        return result;
    } else {
        BigInt abs_this = *this;
        BigInt abs_other = other;
        abs_this.isNegative = false;
        abs_other.isNegative = false;
        auto compare = compare_absolutes(abs_this, abs_other);
        if (compare == std::strong_ordering::equal)
            return BigInt(0);
        BigInt diff;
        if (compare == std::strong_ordering::greater) {
            diff = subtract_unsigned(abs_this, abs_other);
            diff.isNegative = isNegative;
        } else if (compare == std::strong_ordering::less) {
            diff = subtract_unsigned(abs_other, abs_this);
            diff.isNegative = other.isNegative;
        }
        return diff;
    }
}

BigInt BigInt::operator-() const {
    BigInt a = *this;
    a.isNegative = !a.isNegative;
	a.remove_leading_zeros();
    return a;
}

BigInt BigInt::operator-(const BigInt& other) const {
    return *this + (-other);
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt result;
    result.base = base;
    result.digits.resize(digits.size() + other.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); ++i) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            unsigned long long prod = 0;
            if (j < other.digits.size()) {
                prod = digits[i] * other.digits[j];
            }
            unsigned long long sum = result.digits[i + j] + prod + carry;
            result.digits[i + j] = sum % base;
            carry = sum / base;
        }
    }
    result.isNegative = isNegative != other.isNegative;
    result.remove_leading_zeros();
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (other == BigInt(0)) {
        throw std::invalid_argument("Division by zero");
    }
    bool result_isNegative = isNegative != other.isNegative;
    BigInt a = *this;
    a.isNegative = false;
    BigInt b = other;
    b.isNegative = false;
    if (a < b) {
        return BigInt(0);
    }
    BigInt quotient;
    quotient.base = base;
    BigInt current_remainder;
    BigInt bigIntBase = BigInt(base);

    for (unsigned long long & digit : std::ranges::reverse_view(a.digits)) {
        current_remainder = current_remainder * bigIntBase + BigInt(digit);
        BigInt q_i(0);
        BigInt multiple(0);
        while (multiple + b <= current_remainder) {
            multiple += b;
            q_i += BigInt(1);
        }
        current_remainder -= multiple;
        quotient.digits.push_back(q_i.digits.empty() ? 0 : q_i.digits[0]);
    }
    std::reverse(quotient.digits.begin(), quotient.digits.end());
    quotient.remove_leading_zeros();
    quotient.isNegative = !quotient.digits.empty() && result_isNegative;
    return quotient;
}

BigInt BigInt::operator+=(const BigInt& other) { return *this = *this + other; }
BigInt BigInt::operator-=(const BigInt& other) { return *this = *this - other; }
BigInt BigInt::operator*=(const BigInt& other) { return *this = *this * other; }
BigInt BigInt::operator/=(const BigInt& other) { return *this = *this / other; }

BigInt BigInt::operator++() { return *this += BigInt(1); }
BigInt BigInt::operator--() { return *this -= BigInt(1); }

bool BigInt::operator==(const BigInt& other) const {
    return (*this <=> other) == std::strong_ordering::equal;
}

std::strong_ordering BigInt::operator<=>(const BigInt& other) const {
    if (isNegative != other.isNegative) {
        return isNegative ? std::strong_ordering::less
                          : std::strong_ordering::greater;
    }
    const auto abs_comparison = compare_absolutes(*this, other);
    if (isNegative)
        return 0 <=> abs_comparison;
    return abs_comparison;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string input;
    is >> input;
    num = BigInt(input);
    return is;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (num.isNegative && !(num.digits.size() == 1 && num.digits[0] == 0)) {
        os << '-';
    }
    if (num.digits.empty()) {
        os << '0';
    } else {
        os << num.digits.back();
        for (auto it = num.digits.rbegin() + 1; it != num.digits.rend(); ++it) {
            os << std::setw(static_cast<int>(log10(num.base))) << std::setfill('0') << *it;
        }
    }
    return os;
}

BigInt BigInt::operator%(const BigInt& other) const {
	if (other == BigInt(0)) {
		throw std::invalid_argument("Modulo by zero");
	}
	BigInt quotient = *this / other;
	BigInt remainder = *this - quotient * other;
	return remainder;
}

BigInt BigInt::mod_exp(const BigInt& bigIntBase, const BigInt& exp, const BigInt& mod) {
	if (mod <= BigInt(0)) {
		throw std::invalid_argument("mod_exp: modulus must be positive");
	}
	if (mod == BigInt(1)) {
		return BigInt(0);
	}
	if (exp < BigInt(0)) {
		throw std::invalid_argument("mod_exp: negative exponent not supported");
	}

	BigInt result(1);
	BigInt base_mod = bigIntBase % mod;
	if (base_mod < BigInt(0)) {
		base_mod += mod;
	}
	BigInt exponent = exp;

	while (exponent > BigInt(0)) {
		if (exponent % BigInt(2) == BigInt(1)) {
			result = (result * base_mod) % mod;
			if (result < BigInt(0)) {
				result += mod;
			}
		}
		exponent = exponent / BigInt(2);
		base_mod = (base_mod * base_mod) % mod;
		if (base_mod < BigInt(0)) {
			base_mod += mod;
		}
	}

	return result;
}