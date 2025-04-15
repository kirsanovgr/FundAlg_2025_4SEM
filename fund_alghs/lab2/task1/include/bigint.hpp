#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <math.h>
class BigInt {
   public:
	BigInt();
	BigInt(long long value);
	explicit BigInt(const std::string& str);
	BigInt(const BigInt& other);
	BigInt(BigInt&& other) noexcept;
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
	BigInt operator--(int) ;
	BigInt operator++(int) ;
	bool operator==(const BigInt& other) const;
	bool operator!=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;

	friend std::istream& operator>>(std::istream& is, BigInt& num);
	friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

   private:
	std::vector<unsigned long long> digits;
	bool isNegative;
	static const unsigned long long BASE = 1000000000;
	static const int BASE_DIGITS = log10(BASE);
	void removeLeadingZeros();
	static bool validateString(const std::string& str);
	void subtractValue(const BigInt& smaller);
	void addValue(const BigInt& other);
	int compareValue(const BigInt& other) const;
};
