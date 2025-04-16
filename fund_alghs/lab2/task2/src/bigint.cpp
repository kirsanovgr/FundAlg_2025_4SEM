#include "../include/bigint.hpp"

const unsigned long long BigInt::BASE;
const int BigInt::BASE_DIGITS;

bool BigInt::validateString(const std::string& str) {
	if (str.empty()) {
		return false;
	}
	size_t start_pos = 0;
	if (str[0] == '-' || str[0] == '+') {
		if (str.size() == 1) {
			return false;
		}
		start_pos = 1;
	}

	for (size_t i = start_pos; i < str.size(); ++i) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}
bool BigInt::isNull() const {
	if (digits.size() == 1 && digits[0] == 0) {
		return true;
	}
	return false;
}

void BigInt::removeLeadingZeros() {
	while (digits.size() > 1 && digits.back() == 0) {
		digits.pop_back();
	}
	if (isNull()) {
		isNegative = false;
	}
}

void BigInt::subtractValue(const BigInt& smaller) {
	long long borrow = 0;
	size_t n = digits.size();
	size_t m = smaller.digits.size();

	for (size_t i = 0; i < n; ++i) {
		long long current_this_digit = digits[i];
		long long current_smaller_digit = (i < m) ? smaller.digits[i] : 0;
		long long diff = current_this_digit - current_smaller_digit - borrow;

		if (diff < 0) {
			diff += BASE;
			borrow = 1;
		} else {
			borrow = 0;
		}
		digits[i] = static_cast<unsigned long long>(diff);
	}
	removeLeadingZeros();
}
void BigInt::addValue(const BigInt& other) {
	unsigned long long carry = 0;
	size_t n = digits.size();
	size_t m = other.digits.size();
	size_t max_len = std::max(n, m);
	digits.resize(max_len, 0);

	for (size_t i = 0; i < max_len || carry != 0; ++i) {
		if (i == digits.size()) {
			digits.push_back(0);
		}
		unsigned long long other_digit = (i < m) ? other.digits[i] : 0;
		unsigned long long current_sum = digits[i] + other_digit + carry;
		digits[i] = current_sum % BASE;
		carry = current_sum / BASE;
	}
}

int BigInt::compareValue(const BigInt& other) const {
	size_t n = digits.size();
	size_t m = other.digits.size();

	if (n > m) {
		return 1;
	}
	if (n < m) {
		return -1;
	}

	for (int i = n - 1; i >= 0; --i) {
		if (digits[i] > other.digits[i]) {
			return 1;
		}
		if (digits[i] < other.digits[i]) {
			return -1;
		}
	}
	return 0;
}

BigInt::BigInt() : isNegative(false) { digits.push_back(0); }

BigInt::BigInt(long long value) : isNegative(false) {
	if (value == 0) {
		digits.push_back(0);
	} else {
		if (value < 0) {
			isNegative = true;
			value = -value;
		}
	}
	while (value > 0) {
		digits.push_back(static_cast<unsigned long long>(value % BASE));
		value /= BASE;
	}
}

BigInt::BigInt(const std::string& str) : isNegative(false) {
	if (!validateString(str)) {
		throw std::invalid_argument("Invalid string format for BigInt construction");
	}

	size_t start_pos = 0;
	if (str[0] == '-') {
		isNegative = true;
		start_pos = 1;
	} else if (str[0] == '+') {
		isNegative = false;
		start_pos = 1;
	} else {
		isNegative = false;
	}

	size_t first_digit_pos = start_pos;
	while (first_digit_pos < str.length() && str[first_digit_pos] == '0') {
		first_digit_pos++;
	}

	if (first_digit_pos == str.length()) {
		digits.push_back(0);
		isNegative = false;
		return;
	}

	digits.clear();

	for (int i = str.length() - 1; i >= (int)first_digit_pos; i -= BASE_DIGITS) {
		int block_start = std::max((int)first_digit_pos, i - BASE_DIGITS + 1);
		std::string current_part_str = str.substr(block_start, i - block_start + 1);
		try {
			digits.push_back(std::stoull(current_part_str));
		} catch (const std::out_of_range& oor) {
			throw std::out_of_range("Numeric string segment is out of range for unsigned long long: " +
			                        current_part_str);
		}
	}

	removeLeadingZeros();
}
BigInt::BigInt(const BigInt& other) : digits(other.digits), isNegative(other.isNegative) {};

BigInt::BigInt(BigInt&& other) noexcept : digits(std::move(other.digits)), isNegative(other.isNegative) {
	other.isNegative = false;
	other.digits = {0};
}

BigInt& BigInt::operator=(const BigInt& other) {
	if (*this != other) {
		isNegative = other.isNegative;
		digits = other.digits;
	}
	return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
	if (*this != other) {
		isNegative = other.isNegative;
		digits = std::move(other.digits);
		other.isNegative = false;
		other.digits = {0};
	}
	return *this;
}

BigInt& BigInt::operator+=(const BigInt& other) {
	if (isNegative == other.isNegative) {
		addValue(other);
	} else {
		int Value_comparison = compareValue(other);

		if (Value_comparison == 0) {
			digits = {0};
			isNegative = false;
		} else if (Value_comparison > 0) {
			subtractValue(other);
		} else {
			BigInt temp = other;
			temp.subtractValue(*this);
			digits = std::move(temp.digits);
			isNegative = temp.isNegative;
		}
	}
	removeLeadingZeros();
	return *this;
}

// a += (-b)
BigInt& BigInt::operator-=(const BigInt& other) {
	BigInt temp = other;
	if (!(temp.isNull())) {
		temp.isNegative = !temp.isNegative;
	}
	return *this += temp;
}

BigInt BigInt::operator+(const BigInt& other) const { return BigInt(*this) += other; }

BigInt BigInt::operator-(const BigInt& other) const { return BigInt(*this) -= other; }

bool BigInt::operator==(const BigInt& other) const {
	if (isNegative != other.isNegative) {
		return false;
	}
	return compareValue(other) == 0;
}

bool BigInt::operator!=(const BigInt& other) const { return !(*this == other); }

BigInt& BigInt::operator++() { return *this += BigInt(1); }

BigInt BigInt::operator++(int) {
	BigInt temp = *this;
	*this += BigInt(1);
	return temp;
}

BigInt& BigInt::operator--() { return *this -= BigInt(1); }

BigInt BigInt::operator--(int) {
	BigInt temp = *this;
	*this -= BigInt(1);
	return temp;
}

BigInt& BigInt::operator*=(const BigInt& other) {
	bool this_is_zero = isNull();
	bool other_is_zero = other.isNull();
	if (this_is_zero || other_is_zero) {
		*this = BigInt(0);
		return *this;
	}

	bool result_is_negative = (isNegative != other.isNegative);

	size_t n = digits.size();
	size_t m = other.digits.size();
	std::vector<unsigned long long> result_digits(n + m, 0);

	for (size_t i = 0; i < n; ++i) {
		unsigned long long carry = 0;
		for (size_t j = 0; j < m || carry != 0; ++j) {
			unsigned long long current_other_digit = (j < m) ? other.digits[j] : 0;
			unsigned long long current_product = result_digits[i + j] + digits[i] * current_other_digit + carry;
			result_digits[i + j] = current_product % BASE;
			carry = current_product / BASE;
		}
	}

	isNegative = result_is_negative;
	digits = std::move(result_digits);

	removeLeadingZeros();

	return *this;
}

BigInt& BigInt::operator/=(const BigInt& other) {
	if (other.isNull()) {
		throw std::runtime_error("Division by zero");
	}

	bool this_is_zero = isNull();
	if (this_is_zero) {
		return *this;
	}

	int magnitude_cmp = compareValue(other);

	if (magnitude_cmp < 0) {
		*this = BigInt(0);
		return *this;
	}
	if (magnitude_cmp == 0) {
		*this = BigInt(1);
		isNegative = (isNegative != other.isNegative);
		return *this;
	}

	bool result_is_negative = (isNegative != other.isNegative);

	BigInt abs_dividend = *this;
	abs_dividend.isNegative = false;
	BigInt abs_divisor = other;
	abs_divisor.isNegative = false;

	BigInt quotient(0);
	BigInt current_partial_dividend(0);

	for (int i = abs_dividend.digits.size() - 1; i >= 0; --i) {
		current_partial_dividend *= BigInt(BASE);
		current_partial_dividend += BigInt(abs_dividend.digits[i]);

		unsigned long long low = 0, high = BigInt::BASE;
		unsigned long long current_quotient_digit = 0;

		while (low < high) {
			unsigned long long mid = low + (high - low) / 2;
			if (mid == 0) {
				if (abs_divisor <= current_partial_dividend) {
					current_quotient_digit = 1;
				} else {
					current_quotient_digit = 0;
				}
				break;
			}

			BigInt temp_product = abs_divisor;
			temp_product *= BigInt(mid);

			if (temp_product <= current_partial_dividend) {
				current_quotient_digit = mid;
				low = mid + 1;
			} else {
				high = mid;
			}
		}

		quotient *= BigInt(BASE);
		quotient += BigInt(current_quotient_digit);

		if (current_quotient_digit > 0) {
			BigInt to_subtract = abs_divisor;
			to_subtract *= BigInt(current_quotient_digit);
			current_partial_dividend -= to_subtract;
		}
	}

	digits = std::move(quotient.digits);
	isNegative = result_is_negative;

	removeLeadingZeros();

	return *this;
}
BigInt& BigInt::operator%=(const BigInt& num) {
	if (num.isNull()) {
		throw std::runtime_error("Modulo by zero");
	}

	// r = a - (a / b) * b
	BigInt quotient = *this / num;
	*this -= (quotient * num);

	if (isNull()) {
		isNegative = false;
	}

	return *this;
}

BigInt BigInt::operator%(const BigInt& other) const { return BigInt(*this) %= other; }

BigInt BigInt::operator/(const BigInt& other) const { return BigInt(*this) /= other; }
BigInt BigInt::operator*(const BigInt& other) const { return BigInt(*this) *= other; }

bool BigInt::operator>(const BigInt& other) const { return other < *this; }

bool BigInt::operator<(const BigInt& other) const {
	if (isNegative && !other.isNegative) {
		return true;
	}
	if (!isNegative && other.isNegative) {
		return false;
	}

	int magnitude_comparison = compareValue(other);

	if (!isNegative) {
		return magnitude_comparison == -1;
	} else {
		return magnitude_comparison == 1;
	}
}

bool BigInt::operator<=(const BigInt& other) const { return !(*this > other); }
bool BigInt::operator>=(const BigInt& other) const { return !(*this < other); }

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
	if (num.isNull()) {
		os << '0';
		return os;
	}

	if (num.isNegative) {
		os << '-';
	}
	os << num.digits.back();
	for (int i = (int)num.digits.size() - 2; i >= 0; --i) {
		os << std::setw(BigInt::BASE_DIGITS) << std::setfill('0') << num.digits[i];
	}

	return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
	std::string input_str;
	if (is >> input_str) {
		try {
			BigInt temp(input_str);
			num = std::move(temp);
		} catch (...) {
			is.setstate(std::ios_base::failbit);
		}
	}

	return is;
}

BigInt BigInt::mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod) {
	if (mod.isNull() || mod == BigInt(1)) {
		if (mod.isNull()) throw std::runtime_error("Modulo by zero");
		return BigInt(0);
	}
	if (exp.isNegative) {
		throw std::invalid_argument("Negative exp");
	}
	if (exp.isNull()) {
		return BigInt(1);
	}
	if (base.isNull()) {
		return BigInt(0);
	}


	BigInt normalized_base = base % mod;
	if (normalized_base.isNegative) {
		normalized_base += mod;
	}


	BigInt half_exp_result = mod_exp(normalized_base, exp / BigInt(2), mod);

	BigInt result_squared = (half_exp_result * half_exp_result) % mod;
	if (result_squared.isNegative) {
		result_squared += mod;
	}

	if ((exp % BigInt(2)).isNull()) {
		return result_squared;
	} else {
		BigInt final_result = (normalized_base * result_squared) % mod;
		if (final_result.isNegative) {
			final_result += mod;
		}
		return final_result;
	}
}
