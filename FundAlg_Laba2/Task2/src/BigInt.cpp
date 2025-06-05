#include "../include/BigInt.h"

BigInt::BigInt() {
    digits.push_back(0);
    isNegative = false;
    base_osnov = 100000;
}

BigInt::BigInt(long long int value, unsigned long long bs) : BigInt() {
    if (!base_valid(bs)){
        throw std::invalid_argument("incorrect base_osnov");
    }
    base_osnov = bs;
    if (value == 0) {
        return;
    }
    unsigned long long temp;

    if (value < 0) {
        isNegative = true;
        temp = -1 * value;
    } else {
        temp = value;
    }
    digits.clear();
    while (temp != 0) {
        digits.push_back(temp % base_osnov);
        temp = temp / base_osnov;
    }}

void BigInt::string_in_digits(const std::string &in) {
    if (!is_correct_string(in)) {
        throw std::invalid_argument("incorrect input");
    }
    digits.clear();
    isNegative = false;
    std::string tmp_input;
    if (in[0] == '-') {
        isNegative = true;
        tmp_input = in.substr(1);
    } else {
        tmp_input = in;
    }
    auto len_base = static_cast<int> (log10l(base_osnov));
    long long i;
    std::string tmp;
    for (i = (long long)tmp_input.size(); i - len_base >= 0 && i != 0; i = i - len_base) {
        tmp = tmp_input.substr(i - len_base, len_base);
        digits.push_back(std::strtoull(tmp.data(), nullptr, 10));
    }
    if (i > 0) {
        tmp = tmp_input.substr(0, i);
        digits.push_back(std::strtoull(tmp.data(), nullptr, 10));
    }
    remove_leading_zeros();
}

BigInt::BigInt(const std::string &str, unsigned long long bs) : BigInt() {
    if (!base_valid(bs)){
        throw std::invalid_argument("incorrect base_osnov");
    }
    base_osnov = bs;
    string_in_digits(str);
}

BigInt::BigInt(BigInt &&other) noexcept {
    digits = std::move(other.digits);
    base_osnov = other.base_osnov;
    isNegative = other.isNegative;
}

BigInt &BigInt::operator=(const BigInt &other) {
    if (this != &other) {
        digits = other.digits;
        base_osnov = other.base_osnov;
        isNegative = other.isNegative;
    }
    return *this;
}

BigInt &BigInt::operator=(BigInt &&other) noexcept {
    if (this != &other) {
        digits = std::move(other.digits);
        base_osnov = other.base_osnov;
        isNegative = other.isNegative;
    }
    return *this;
}

bool BigInt::operator==(const BigInt &other) const {
    BigInt temp(other);
    temp.change_base(base_osnov);
    return (isNegative == other.isNegative) && (digits == temp.digits);
}

bool BigInt::operator!=(const BigInt &other) const {
    return !(*this == other);
}

bool BigInt::is_correct_string(const std::string &str) {
    if (str.empty()) return false;
    for (size_t i = 0; i < str.size(); i++) {
        char c = str[i];
        if (c == '-' && i == 0) {
            continue;
        }
        if (c < '0' || c > '9')
            return false;
    }
    return true;
}

std::string BigInt::digits_in_string() const {
    auto temp_copy = digits;
    auto len_num = static_cast<size_t> (log10l(base_osnov));
    std::stringstream output;
    if (isNegative) {
        output << '-';
    }
    auto num = temp_copy.back();
    temp_copy.pop_back();
    output << num;

    while (!temp_copy.empty()) {
        num = temp_copy.back();
        temp_copy.pop_back();
        std::string str = std::to_string(num);
        if (str.length() < len_num) {
            str.insert(0, len_num - str.length(), '0');
        }
        output << str;
    }
    return output.str();
}

void BigInt::change_base(unsigned long long int new_base) {
    if (new_base == base_osnov) {
        return;
    }
    BigInt temp(0, new_base);
    for (size_t i = digits.size(); i > 0; i--) {
        temp = temp.mul_size_t(base_osnov);
        temp.unsign_sum(digits[i - 1]);
    }
    temp.isNegative = isNegative;
    *this = temp;
}

std::strong_ordering BigInt::operator<=>(const BigInt &other) const {
    if (isNegative == other.isNegative) {
        BigInt temp(other);
        temp.change_base(base_osnov);
        if (temp.digits.size() == digits.size()) {
            for (size_t i = digits.size(); i > 0; i--) {
                if (digits[i - 1] != temp.digits[i - 1]) {
                    return isNegative ^ (digits[i - 1] < temp.digits[i - 1]) ? std::strong_ordering::less
                                                                             : std::strong_ordering::greater;
                }
            }
            return std::strong_ordering::equivalent;
        }
        return isNegative ? temp.digits.size() <=> digits.size() : digits.size() <=> temp.digits.size();
    }

    return isNegative ? std::strong_ordering::less : std::strong_ordering::greater;
}

BigInt BigInt::abs_num() const {
    BigInt res(*this);
    res.isNegative = false;
    return res;
}

BigInt BigInt::operator+(const BigInt &other) const {
    BigInt res;
    int sign = isNegative ? -1 : 1;
    sign *= other.isNegative ? -1 : 1;
    if (sign == 1) {
        res = other;
        res.change_base(base_osnov);
        res.unsign_sum(*this);
        res.isNegative = isNegative;
        return res;
    }
    if (abs_num() > other.abs_num()) {
        res = *this;
        res.change_base(other.base_osnov);
        res.unsign_sub(other);
        return res;
    }
    res = other;
    res.change_base(base_osnov);
    res.unsign_sub(*this);
    return res;}

void BigInt::unsign_sum(const BigInt &a1) { // Прибавление другого igInt с той же базой без знака (промежуточный результат)
    size_t max_size = std::max(digits.size(), a1.digits.size());
    digits.resize(max_size, 0);
    unsigned long long carry = 0;

    for (size_t i = 0; i < max_size; ++i) {
        unsigned long long sum = digits[i] + carry;
        if (i < a1.digits.size()) sum += a1.digits[i];

        carry = sum / base_osnov;
        digits[i] = sum % base_osnov;
    }

    if (carry) digits.push_back(carry);
}

void BigInt::unsign_sub(const BigInt &a1) { // Вычитание из большего меньшее неотрицательных чисел при одной базе
    unsigned long long um = 0;
    size_t i;
    for (i = 0; i < digits.size() && i < a1.digits.size(); i++) {
        unsigned long long mul_num;
        if (digits[i] < a1.digits[i] + um) {
            mul_num = digits[i] + base_osnov - a1.digits[i] - um;
            um = 1;

        } else {
            mul_num = digits[i] - a1.digits[i] - um;
            um = 0;
        }
        digits[i] = mul_num;
    }
    for (; i < digits.size() && um != 0; i++) {
        unsigned long long mul_num;
        if (digits[i] < um) {
            mul_num = digits[i] + base_osnov - um;
            um = 1;

        } else {
            mul_num = digits[i] - um;
            um = 0;
        }
        digits[i] = mul_num;
    }
    remove_leading_zeros();
}

BigInt BigInt::operator-(const BigInt &other) const {
    BigInt temp(other);
    temp.isNegative = !temp.isNegative;
    return ((*this) + temp);
}

BigInt BigInt::mul_size_t(unsigned long long int num) const {
    if (num == 0) {
        return BigInt(0, base_osnov);
    }
    BigInt res(0, base_osnov);
    res.digits.clear();
    unsigned long long um = 0;
    for (unsigned long long digit: digits) {
        unsigned long long mul_num = digit * num + um;
        res.digits.push_back(mul_num % base_osnov);
        um = mul_num / base_osnov;
    }
    while (um > 0) {
        res.digits.push_back(um % base_osnov);
        um /= base_osnov;
    }
    return res;
}

BigInt BigInt::operator*(const BigInt &other) const {
    BigInt res(0, base_osnov);
    BigInt m(other);
    m.change_base(base_osnov);
    for (size_t i = digits.size(); i > 0 ; i--) {
        res = res.mul_size_t(base_osnov);
        res.unsign_sum(m.mul_size_t(digits[i - 1]));
    }
    res.isNegative = isNegative ^ other.isNegative;
    return res;
}

std::pair<BigInt, BigInt> BigInt::del_num(const BigInt &other) const {
    if (other == BigInt(0)) {
        throw std::domain_error("Dividing by 0");
    }
    BigInt quotient(0, base_osnov), remainder(0, base_osnov), delitel(other);
    delitel.isNegative = false;

    if (*this < other) {
        return {BigInt(0), *this};
    }

    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
        remainder.digits.insert(remainder.digits.cbegin(), digits[i]);
        remainder.remove_leading_zeros();
        unsigned long long count = 0;
        while (remainder >= delitel) {
            remainder -= delitel;
            count++;
        }
        quotient.digits.insert(quotient.digits.cbegin(), count);
        quotient.remove_leading_zeros();
    }
    quotient.remove_leading_zeros();
    remainder.remove_leading_zeros();
    quotient.isNegative = isNegative ^ other.isNegative;
    remainder.isNegative = false;
    return {quotient, remainder};
}

BigInt BigInt::operator/(const BigInt &other) const {
    BigInt res(0, base_osnov);
    res.isNegative = isNegative ^ other.isNegative;

    BigInt a(*this);
    a.isNegative = false;
    BigInt b(other);
    b.isNegative = false;
    b.change_base(base_osnov);
    auto res_ost = a.del_num(b);
    res.digits = res_ost.first.digits;
    res.base_osnov = res_ost.first.base_osnov;
    return res;

}

BigInt BigInt::operator+=(const BigInt &other) {
    *this = *this + other;
    return *this;
}

BigInt BigInt::operator-=(const BigInt &other) {
    *this = *this - other;
    return *this;
}

BigInt BigInt::operator*=(const BigInt &other) {
    *this = *this * other;
    return *this;
}

BigInt BigInt::operator/=(const BigInt &other) {
    *this = *this / other;
    return *this;
}

BigInt BigInt::operator++() {
    *this += BigInt(1, 100000);
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp(*this);
    *this += BigInt(1, 100000);
    return temp;}

BigInt BigInt::operator--() {
    *this -= BigInt(1, 100000);
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp(*this);
    *this -= BigInt(1, 100000);
    return temp;}

std::istream &operator>>(std::istream &is, BigInt &num) {
    std::string str;
    is >> str;
    num.string_in_digits(str);
    return is;

}

std::ostream &operator<<(std::ostream &os, const BigInt &num) {
    os << num.digits_in_string();
    return os;
}



BigInt BigInt::operator%(const BigInt &other) const {
    BigInt res;
    res.isNegative = isNegative;

    BigInt a(*this);
    a.isNegative = false;
    BigInt b(other);
    b.isNegative = false;
    b.change_base(base_osnov);
    auto res_ost = a.del_num(b);
    res.digits = res_ost.second.digits;
    res.base_osnov = res_ost.second.base_osnov;
    return res;
}

BigInt BigInt::operator%=(const BigInt &other) {
    BigInt res = *this % other;
    *this = res;
    return *this;
}

BigInt BigInt::operator*(const long long int num) const {
    BigInt res(0, base_osnov);
    unsigned long long t_num = 0;
    if (num < 0) {
        t_num = -num;
    }
    else {
        t_num = num;
    }
    res = mul_size_t(t_num);
    res.isNegative = (num < 0) ^ isNegative;
    return res;}

BigInt BigInt::operator*(const unsigned long long int num) const {
    BigInt res;
    res.isNegative = isNegative;
    res = mul_size_t(num);
    return res;}

void BigInt::unsign_sum(unsigned long long int num) {
    unsigned long long um = num;
    size_t i;
    for (i = 0; i < digits.size() && 0 < um; i++) {
        unsigned sum_num = digits[i] + um;
        um = sum_num / base_osnov;
        digits[i] = sum_num % base_osnov;
    }
    while (um > 0) {
        digits.push_back(um % base_osnov);
        um = um / base_osnov;
    }
}

void BigInt::remove_leading_zeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.size() == 1 && digits[0] == 0) {
        isNegative = false;
    }
}

BigInt BigInt::operator-() const {
    BigInt a = *this;
    a.isNegative = !a.isNegative;
    return a;
}

bool BigInt::base_valid(unsigned long long int test_base) {
    if (test_base == 0) {
        return false;
    }
    auto len_base = (int) log10l(test_base);

    return std::abs(log10l(test_base) - len_base) < 1e-15;
}

BigInt mod_exp(const BigInt &base, const BigInt &exp, const BigInt &mod) {
    if (exp < BigInt(0)){
        throw std::invalid_argument("exp < 0");
    }
    BigInt result(1);
    BigInt a = base;
    BigInt b = exp;

    a = a % mod;

    BigInt zero(0);
    BigInt one(1);
    BigInt two(2);

    while (b > zero) {
        if (b % two == one) {
            result = (result * a) % mod;
        }
        a = (a * a) % mod;

        b /= BigInt(2);
    }

    return result;
}