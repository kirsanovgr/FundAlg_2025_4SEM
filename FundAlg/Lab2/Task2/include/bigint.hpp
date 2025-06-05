#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <vector>

class BigInt {
private:
	std::vector<unsigned long long> digits{};
	bool isNegative = false;
	unsigned long long base = 100;

	void parse_unsigned_value(unsigned long long value);

	void remove_leading_zeros();

	static std::strong_ordering compare_absolutes(const BigInt& a, const BigInt& b);

	[[nodiscard]] BigInt subtract_unsigned(const BigInt& a, const BigInt& b) const;

public:
	BigInt() = default;

	explicit BigInt(long long value);

	explicit BigInt(const std::string& str);

	BigInt(const BigInt& other);

	BigInt(BigInt&& other) noexcept;

	~BigInt() = default;

	BigInt& operator=(const BigInt& other);

	BigInt& operator=(BigInt&& other) noexcept;

	BigInt operator+(const BigInt& other) const;

	BigInt operator-() const;

	BigInt operator-(const BigInt& other) const;

	BigInt operator*(const BigInt& other) const;

	BigInt operator/(const BigInt& other) const;

	BigInt operator%(const BigInt& other) const;

	BigInt operator+=(const BigInt& other);

	BigInt operator-=(const BigInt& other);

	BigInt operator*=(const BigInt& other);

	BigInt operator/=(const BigInt& other);

	BigInt operator++();

	BigInt operator--();

	bool operator==(const BigInt& other) const;

	std::strong_ordering operator<=>(const BigInt& other) const;

	friend std::istream& operator>>(std::istream& is, BigInt& num);

	friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

	static BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod);
};

#endif //BIGINT_HPP