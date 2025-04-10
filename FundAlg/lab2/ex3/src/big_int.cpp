#include "../include/big_int.hpp"

BigInt::BigInt(long long int l) : BigInt() {
    if (l == 0) {
        return;
    }
    if (l < 0) {
        is_negative = true;
        l *= -1;
    }
    data.pop_back();
    while (l > 0) {
        data.push_back(l % base);
        l /= base;
    }
} // LCOV_EXCL_LINE

BigInt::BigInt(const std::string &in) : BigInt() {
    reload_from_string(in);
}


std::ostream &operator<<(std::ostream &out, const BigInt &num) {
    out << num.to_string();
    return out;
}

void BigInt::remove_leading_zeros() {
    if (is_null() and is_negative) {
        is_negative = false;
    }
    for (int i = data.size() - 1; i > 0; --i) {
        if (data[i] == 0) {
            data.pop_back();
        } else {
            return;
        }
    }
}

std::string BigInt::to_string() const {
    std::stringstream output;
    if (is_negative) {
        output << "-";
    }
    auto len_base = (int) log10l(base);
    for (int i = data.size() - 1; i >= 0; --i) {
        if ((size_t) i != data.size() - 1) {
            output << std::setw(len_base) << std::setfill('0') << data[i];
        } else {
            output << data[i];
        }
    }
    std::string s;
    output >> s;
    return s;
}

std::istream &operator>>(std::istream &in, BigInt &num) {
    std::string tmp;
    in >> tmp;
    num = BigInt{tmp};
    return in;
}

BigInt &BigInt::operator=(const BigInt &other) {
    is_negative = other.is_negative;
    base = other.base;
    data.resize(other.data.size());
    std::copy(other.data.begin(), other.data.end(), data.begin());
    return *this;
}

BigInt &BigInt::operator=(BigInt &&other) noexcept {
    if (this != &other) {
        *this = other;
        other.data.clear();
        other.is_negative = false;
    }
    return *this;
}

BigInt::BigInt(BigInt &&other) noexcept: BigInt() {
    if (this != &other) {
        std::swap(this->data, other.data);
        std::swap(this->is_negative, other.is_negative);
        std::swap(this->base, other.base);
    }
}

BigInt::BigInt(const BigInt &other) : BigInt() {
    if (this != &other) {
        *this = other;
    }
}

bool BigInt::is_correct_string(const std::string &str) {
    for (size_t i = 0; i < str.size(); ++i) {
        if ((str[i] < '0' or str[i] > '9') and (i != 0 or str[i] != '-')) {
            return false;
        }
    }
    if (str.length() == 1 and str[0] == '-') {
        return false;
    }
    return true;
}

void BigInt::change_base(unsigned long long int new_base) {
    if(new_base == base) {
        return;
    }
    if (new_base == 0) {
        throw std::invalid_argument("base cannot be zero");
    }
    auto len_base = (unsigned long long) log10l(new_base);

    if (log10l(new_base) - len_base > 1e-15) {
        throw std::invalid_argument("incorrect new base: should be pow of 10");
    }
    std::string tmp = to_string();
    base = new_base;
    reload_from_string(tmp);
    remove_leading_zeros();
}

void BigInt::reload_from_string(const std::string &in) {

    if (!is_correct_string(in)) {
        throw std::invalid_argument("incorrect input");
    }
    data.clear();
    is_negative = false;
    std::string tmp_input;
    if (in[0] == '-') {
        is_negative = true;
        tmp_input = in.substr(1);
    } else {
        tmp_input = in;
    }

    auto len_base = (int) log10l(base);
    int i;
    std::string tmp;
    for (i = tmp_input.size(); i - len_base >= 0; i = i - len_base) {
        tmp = tmp_input.substr(i - len_base, len_base);

        data.push_back(std::strtoull(tmp.data(), nullptr, 10));
    }
    if (i > 0) {
        tmp = tmp_input.substr(0, i);
        data.push_back(std::strtoull(tmp.data(), nullptr, 10));
    }

    remove_leading_zeros();
}

std::strong_ordering operator<=>(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.is_negative and !rhs.is_negative) {
        return std::strong_ordering::less;
    } else if (!lhs.is_negative and rhs.is_negative) {
        return std::strong_ordering::greater;
    }
    std::string s1 = lhs.to_string();
    std::string s2 = rhs.to_string();
    if (s1.size() > s2.size()) {
        return std::strong_ordering::greater;
    } else if (s1.size() < s2.size()) {
        return std::strong_ordering::less;
    }
    for (size_t i = 0; i < s1.size(); ++i) {
        if (s1[i] > s2[i]) {
            return std::strong_ordering::greater;
        } else if (s1[i] < s2[i]) {
            return std::strong_ordering::less;
        }
    }
    return std::strong_ordering::equal;
}

bool operator==(const BigInt &lhs, const BigInt &rhs) {
    return (lhs <=> rhs) == std::strong_ordering::equal;
}

BigInt BigInt::mod_exp(const BigInt &base, const BigInt &exp, const BigInt &mod) {
    if (exp.is_null()) {
        return BigInt{1};
    }
    BigInt a = mod_exp(base % mod, exp / BigInt(2), mod) % mod;
    if (exp % BigInt{2} == BigInt{0}) {
        return (a * a) % mod;
    } else {
        return ((base % mod) * ((a * a) % mod)) % mod;
    }
}

BigInt BigInt::operator+(const BigInt &num) const {
    BigInt tmp{*this};
    return tmp += num;
}

BigInt BigInt::operator-() {
    BigInt r{*this};
    r.is_negative = !is_negative;
    return r;
}

BigInt BigInt::operator-(const BigInt &num) const {
    BigInt tmp{*this};
    return tmp -= num;
}

BigInt BigInt::operator*(const BigInt &num) const {
    BigInt tmp{*this};
    return tmp *= num;
}

BigInt BigInt::operator/(const BigInt &num) const {
    BigInt tmp{*this};
    return tmp /= num;
}

bool BigInt::is_null() const {
    return (data.size() == 1 and data[0] == 0) or data.size() == 0;
}


BigInt::BigInt() : base(1000000000), data({0}), is_negative(false) {}

BigInt &BigInt::operator+=(const BigInt &num) {
    BigInt rhs{num};
    rhs.change_base(base);

    BigInt lhs{*this};
    if (is_negative) {
        if (rhs.is_negative) {
            *this = -((-rhs) + (-lhs));
            return *this;
        }
        *this = rhs - (-lhs);
        return *this;
    } else if (rhs.is_negative) {
        *this = lhs - (-rhs);
        return *this;
    }
    BigInt res;
    res.change_base(base);
    res.data.pop_back();

    unsigned long long carry = 0;
    size_t i = 0;
    for (; i < std::max(rhs.data.size(), data.size()); ++i) {
        if (i < rhs.data.size() && i < data.size()) {
            res.data.push_back((rhs.data[i] + data[i] + carry) % base);
            carry = (rhs.data[i] + data[i] + carry) / base;
        } else if (i < data.size()) {
            res.data.push_back((data[i] + carry) % base);
            carry = (data[i] + carry) / base;
        } else {
            res.data.push_back((rhs.data[i] + carry) % base);
            carry = (rhs.data[i] + carry) / base;
        }
    }
    while (carry > 0) {
        res.data.push_back(carry % base);
        carry /= base;
    }

    res.remove_leading_zeros();
    *this = res;
    return *this;
}

BigInt &BigInt::operator-=(const BigInt &num) {
    if (this == &num) {
        *this = BigInt(0);
        return *this;
    }
    BigInt t{num};
    if (base != num.base) {
        BigInt tmp = num;
        tmp.change_base(base);
        return *this -= tmp;
    }

    if (num.is_negative) {
        return *this += (-t);
    }
    if (is_negative) {
        *this = -((-*this) + num);
        return *this;
    }
    if (*this < num) {
        BigInt res = num - *this;
        res.is_negative = true;
        *this = res;
        return *this;
    }
    long long carry = 0;
    for (size_t i = 0; i < num.data.size() || carry; ++i) {
        const long long num_digit = (i < num.data.size()) ? num.data[i] : 0;


        long long diff = data[i] - num_digit - carry;
        carry = 0;

        if (diff < 0) {
            diff += base;
            carry = 1;
        }

        data[i] = diff;
    }


    remove_leading_zeros();
    return *this;
}

BigInt &BigInt::operator*=(const BigInt &num) {
    BigInt tmp{*this};
    BigInt tmp2{num};
    tmp2.change_base(base);
    BigInt res;
    res.change_base(base);
    res.data.resize(tmp.data.size() + tmp2.data.size());
    if (is_negative) {
        if (num.is_negative) {
            *this = (-tmp) * (-tmp2);
            remove_leading_zeros();
            return *this;
        }
        *this = -((-tmp) * tmp2);
        remove_leading_zeros();
        return *this;
    }
    if (num.is_negative) {
        *this = -((-tmp2) * tmp);
        remove_leading_zeros();
        return *this;
    }
    unsigned long long carry = 0;

    for (size_t i = 0; i < tmp.data.size(); ++i) {
        for (size_t j = 0; j < tmp2.data.size() or carry; ++j) {
            unsigned long long cur;
            if (j < tmp2.data.size()) {
                cur = res.data[i + j] + tmp.data[i] * tmp2.data[j] + carry;
            } else {
                cur = res.data[i + j] + carry;
            }
            res.data[i + j] = cur % base;
            carry = cur / base;
        }
    }

    res.remove_leading_zeros();
    *this = res;
    return *this;
}

BigInt &BigInt::operator/=(const BigInt &num) {
    BigInt dividend{*this};
    BigInt divisor{num};
    if (is_negative) {
        if (num.is_negative) {
            *this = (-dividend) / (-divisor);
            remove_leading_zeros();
            return *this;
        }
        *this = -((-dividend) / divisor);
        remove_leading_zeros();
        return *this;
    } else if (num.is_negative) {
        *this = -(dividend / (-divisor));
        remove_leading_zeros();
        return *this;
    }
    divisor.change_base(dividend.base);
    std::pair<BigInt, BigInt> res = divide(dividend, divisor);
    *this = res.first;
    remove_leading_zeros();
    return *this;
}

BigInt &BigInt::operator++() {
    return *this += BigInt(1);
}

BigInt &BigInt::operator--() {
    return *this -= BigInt(1);
}

BigInt BigInt::operator++(int) {
    BigInt tmp{*this};
    *this += BigInt(1);
    return tmp;
} // LCOV_EXCL_LINE

BigInt BigInt::operator--(int) {
    BigInt tmp{*this};
    *this -= BigInt(1);
    return tmp;
} // LCOV_EXCL_LINE

BigInt &BigInt::operator%=(const BigInt &num) {
    BigInt tmp = *this / num;
    *this = *this - tmp * num;
    if (is_negative and num.is_negative) {
        is_negative = false;
    }
    return *this;
}

BigInt BigInt::operator%(const BigInt &num) const {
    BigInt tmp{*this};
    return tmp %= num;
}

BigInt BigInt::fft_multiply(const BigInt &a) const {
    BigInt lhs{*this};
    BigInt rhs{a};

    if (lhs.is_negative) {
        if (rhs.is_negative) {
            lhs.is_negative = false;
            return lhs.fft_multiply(-rhs);
        } else {
            lhs.is_negative = false;
            return -lhs.fft_multiply(rhs);
        }
    } else if (rhs.is_negative) {
        return -lhs.fft_multiply(-rhs);
    }

    rhs.change_base(lhs.base);
    size_t n = 1;
    size_t total_size = lhs.data.size() + rhs.data.size();
    while (n < 2 * total_size) n <<= 1;

    auto dft1 = fft(n, fft_transform(lhs.data, n), omega(n, 1));
    auto dft2 = fft(n, fft_transform(rhs.data, n), omega(n, 1));

    std::vector<std::complex<long double>> tmp_res(n);
    for (size_t i = 0; i < n; ++i) {
        tmp_res[i] = dft1[i] * dft2[i];
    }

    auto dft3 = fft(n, tmp_res, omega(n, -1));
    for (auto& x : dft3) x /= n;

    BigInt res;
    res.data = fft_reset(dft3, lhs.base);
    res.base = lhs.base;
    return res;
}


std::vector<std::complex<long double>> BigInt::fft(size_t n, std::vector<std::complex<long double>> f, std::complex<long double> w) {
    if (n == 1) {
        return f;
    }

    std::vector<std::complex<long double>> r1(n / 2), r2(n / 2);
    std::complex<long double> current_w(1, 0);
    for (size_t i = 0; i < n / 2; ++i) {
        r1[i] = f[i] + f[i + n / 2];
        r2[i] = (f[i] - f[i + n / 2]) * current_w;
        current_w *= w;
    }
    std::vector<std::complex<long double>> a = fft(n / 2, r1, w * w);
    std::vector<std::complex<long double>> b = fft(n / 2, r2, w * w);


    std::vector<std::complex<long double>> res;
    for(size_t i = 0; i < n / 2; ++i) {
        res.push_back(a[i]);
        res.push_back(b[i]);
    }

    return res;
}

std::complex<long double> BigInt::omega(int n, int k) {
    long double angle = -2 * M_PIl * k / n;
    return {cos(angle), sin(angle)};
}

std::vector<std::complex<long double>> BigInt::fft_transform(const std::vector<unsigned long long>& input, size_t n) {
    std::vector<std::complex<long double>> complex_arr(n);
    for (size_t i = 0; i < input.size(); ++i) {
        complex_arr[i] = input[i];
    }
    return complex_arr;
}

std::vector<unsigned long long> BigInt::fft_reset(const std::vector<std::complex<long double>>& input, size_t base) {
    std::vector<unsigned long long> res;
    for (auto el : input) {

        unsigned long long value = std::llround(el.real() + 1e-8L);
        res.push_back(value);
    }
    BigInt tmp;
    tmp.base = base;
    tmp.data = res;
    tmp.normalize();
    return tmp.data;
}

void BigInt::normalize() {
    unsigned long long carry = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] += carry;
        carry = data[i] / base;
        data[i] %= base;
    }
    while (carry > 0) {
        data.push_back(carry % base);
        carry /= base;
    }
    remove_leading_zeros();
}

void BigInt::shift_left(int k) {
    if(k > 0) {
        data.insert(data.begin(), k, 0);
    } else if(k < 0) {
        for (int i = 0; i < abs(k); ++i) {
            data.erase(data.begin());
        }
    }
}

/*
 * 4234324
 * 369____
 *  54
 */

std::pair<BigInt, BigInt> BigInt::divide(const BigInt &lhs, const BigInt &rhs) {
    if (rhs.is_null()) {
        throw std::invalid_argument("denominator should be not 0");
    }
    BigInt remainder;
    BigInt quotient;
    quotient.change_base(lhs.base);
    remainder.change_base(lhs.base);

    BigInt dividend{lhs};
    BigInt divisor{rhs};

    while (true) {

        if(dividend < rhs) {
            break;
        }
        divisor.shift_left(dividend.data.size() - divisor.data.size());
        while (dividend < divisor) {
            quotient.data.push_back(0);
            divisor.shift_left(-1);
        }
        BigInt tmp{divisor};
        size_t k = 1;
        while (divisor + tmp <= dividend) {
            divisor += tmp;
            ++k;
        }
        dividend -= divisor;
        quotient.data.push_back(k);
        divisor = rhs;
    }
    std::reverse(quotient.data.begin(), quotient.data.end());
    quotient.normalize();
    remainder = dividend;
    return {quotient, remainder};
}