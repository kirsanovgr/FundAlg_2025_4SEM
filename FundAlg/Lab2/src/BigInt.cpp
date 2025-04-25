#include "../include/BigInt.h"

BigInt::BigInt() : digits(), isNegative() {
    digits.push_back(0);
}

BigInt::BigInt(long long value) : BigInt() {
    digits.pop_back();
    if (value < 0) {
        isNegative = true;
        value = -value;
    }
    else if (value == 0) {
        digits.push_back(0);
        return;
    }
    while(value > 0) {
        digits.push_back(value % base);
        value /= base;
    }
// баг покрытия со скобочкой
} // LCOV_EXCL_LINE

BigInt::BigInt(const std::string& str) : BigInt() {
    digits.pop_back();
    size_t minus_checker = 0;
    int base_len = 5;
    int i;
    for (i = str.size(); i - base_len >= 0; i -= base_len) {
        std::string tmp = str.substr(i - base_len, base_len);
        for (unsigned long long j = 0; j < tmp.size(); j++) {
            if (tmp[j] == '-' && (i - base_len) == 0 && j == 0) {
                isNegative = true;
                minus_checker = 1;
            }
            else if (!isdigit(tmp[j])) {
                throw std::invalid_argument("incorrect string argument");
            }
        }
        digits.push_back(std::strtoull(tmp.data() + minus_checker, nullptr, 10));
        minus_checker = 0;
    

    }
    if (i != 0) {
        std::string tmp = str.substr(0, i);
        for (unsigned long long j = 0; j < tmp.size(); j++) {
            if (j == 0 && tmp[j] == '-') {
                isNegative = true;
                minus_checker = 1;
            }
            else if (!isdigit(tmp[j])) {
                throw std::invalid_argument("incorrect string argument");
            }
        }
        if (tmp.size() != minus_checker) {
            digits.push_back(std::strtoull(tmp.data() + minus_checker, nullptr, 10));
        }
    }
    if (digits.size() == 0) {
        throw std::invalid_argument("incorrect string argument");
    }

}

BigInt::BigInt(const BigInt& other) : digits(other.digits), isNegative(other.isNegative) {}

BigInt::BigInt(BigInt&& other) noexcept : digits(std::move(other.digits)), isNegative(std::move(other.isNegative)) {
    other.digits.clear();
    other.digits.push_back(0);
    other.isNegative = false;
}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        digits.resize(other.digits.size());
        std::copy(other.digits.begin(), other.digits.end(), digits.begin());
        isNegative = other.isNegative;
    }
    return *this;

}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        digits.resize(other.digits.size());
        digits = std::move(other.digits);
        isNegative = std::move(other.isNegative);
        other.digits.clear();
        other.isNegative = false;
        other.digits.push_back(0);
    }
    return *this;
    
}

BigInt BigInt::operator+(const BigInt& other) const {
    // если разные знаки, то в оператор -
    if (this->isNegative && !(other.isNegative)) {
        BigInt tmp = *this;
        tmp.isNegative = false;
        return other - tmp;
    }
    else if (!(this->isNegative) && other.isNegative) {
        BigInt tmp = other;
        tmp.isNegative = false;
        return *this - tmp;
    }
    BigInt ans = *this;
    unsigned long long for_next = 0;
    // складывем блоки с переносом
    for (size_t i = 0; i < std::min(digits.size(), other.digits.size()); i++) {
        unsigned long long current_sum = ans.digits[i] + other.digits[i] + for_next;
        ans.digits[i] = current_sum % base;
        for_next = current_sum / base;
    }
    if (std::min(digits.size(), other.digits.size()) == other.digits.size()) {
        for (size_t i = other.digits.size(); i < digits.size(); i++) {
            unsigned long long current_sum = ans.digits[i] + for_next;
            ans.digits[i] = current_sum % base;
            for_next = current_sum / base;
            if (for_next == 0) {
                break;
            }
        }
    }
    // докидываем остатки, если есть
    for (size_t i = ans.digits.size(); i < other.digits.size(); i++) {
        unsigned long long current_sum = other.digits[i] + for_next;
        ans.digits.push_back(current_sum % base);
        for_next = current_sum / base;
    }
    if (for_next != 0) {
        ans.digits.push_back(for_next);
    }
    return ans;


}

BigInt BigInt::operator-(const BigInt& other) const {
    // если знаки разные, то в оператор +
    if (this->isNegative && !(other.isNegative)) {
        BigInt tmp = other;
        tmp.isNegative = true;

        return *this + tmp;
    }
    else if (!(this->isNegative) && other.isNegative) {
        BigInt tmp = other;
        tmp.isNegative = false;
        return *this + tmp;
    }
    if (other > *this) {
        BigInt ans = other - *this;
        ans.isNegative = !(other.isNegative);
        return ans;
    }
    BigInt ans = *this;
    unsigned long long from_next = 0;
    unsigned long long last_from_next = 0;
    for (size_t i = 0; i < std::min(digits.size(), other.digits.size()); i++) {
        if (ans.digits[i] < other.digits[i] + from_next) {
            last_from_next = from_next;
            from_next = 1;
        }
        else {
            from_next = 0;
        }
        ans.digits[i] = ans.digits[i] + base * from_next - other.digits[i] - last_from_next;
        last_from_next = from_next;
        //std::cout << ans.digits[i] << "\n";

    }
    for(size_t i = other.digits.size(); i < ans.digits.size(); i++) {
        if (ans.digits[i] < from_next) {
            ans.digits[i] = base - from_next;
            from_next = 1;
        }
        else {
            ans.digits[i] = ans.digits[i] - from_next;
            from_next = 0;
        }
    }
    while (ans.digits[ans.digits.size() - 1] == 0 && ans.digits.size() != 1) {
        ans.digits.pop_back();
    }
    if (ans.digits[ans.digits.size() - 1] == 0) {
        ans.isNegative = false;
    }
    return ans;




}

std::strong_ordering BigInt::operator<=>(const BigInt &rhs) const {
    if (isNegative && !rhs.isNegative) {
        return std::strong_ordering::less;
    } else if (!isNegative && rhs.isNegative) {
        return std::strong_ordering::greater;
    }
    if (this->digits.size() > rhs.digits.size()) {
        return std::strong_ordering::greater;
    } else if (this->digits.size() < rhs.digits.size()) {
        return std::strong_ordering::less;
    }
    for (size_t i = 0; i < this->digits.size(); i++) {
        if (this->digits[this->digits.size() - 1 - i] > rhs.digits[this->digits.size() - 1 - i]) {
            return std::strong_ordering::greater;
        } else if (this->digits[this->digits.size() - 1 - i] < rhs.digits[this->digits.size() - 1 - i]) {
            return std::strong_ordering::less;
        }
    }
    return std::strong_ordering::equal;
}

bool BigInt::operator==(const BigInt &rhs) const {
    return (*this <=> rhs) == std::strong_ordering::equal;
}

BigInt& BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;

}

BigInt& BigInt::operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;

}

BigInt& BigInt::operator++() {
    *this += BigInt(1);
    return *this;
}

BigInt& BigInt::operator--() {
    *this -= BigInt(1);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (num.isNegative) {
        os << '-';
    }
    for (size_t i = 0; i < num.digits.size(); i++) {
        std::string current = std::to_string(num.digits[num.digits.size() - 1 - i]);
        if ((i != 0) && (current.size() != 5)) {
            for (size_t j = 0; j < 5 - current.size(); j++) {
                os << '0';
            }
        }
        os << current;
    }
    return os;
}

std::istream& operator>>(std::istream& in, BigInt& num) {
    std::string tmp;
    in >> tmp;
    num = BigInt(tmp);
    return in;

}

std::string BigInt::to_string() const {
    std::string ans;
    if (isNegative) {
        ans += '-';
    }
    for (size_t i = 0; i < digits.size(); i++) {
        std::string current = std::to_string(digits[digits.size() - 1 - i]);
        if ((i != 0) && (current.size() != 5)) {
            for (size_t j = 0; j < 5 - current.size(); j++) {
                ans += '0';
            }
        }
        ans += current;
    }
    return ans;
} // LCOV_EXCL_LINE

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt ans;
    ans.digits.pop_back();
    if (this->isNegative == other.isNegative) {
        ans.isNegative = false;
    }
    else {
        ans.isNegative = true;
    }
    // длина результата = длина 1 + длина 2 - e (e = 0 или e = 1)
    ans.digits.resize(this->digits.size() + other.digits.size());
    unsigned long long for_next = 0;
    unsigned long long current_sum;
    for (size_t i = 0; i < this->digits.size(); i++) {
        for (size_t j = 0; j < other.digits.size(); j++) {
            // позиция результата = сумме позиции перемножаемых "разрядов"
            current_sum = ans.digits[i + j] + this->digits[i] * other.digits[j] + for_next;
            ans.digits[i + j] = current_sum % base;
            for_next = current_sum / base;

        }
        // докидываем остаток
        int count = other.digits.size();
        while (for_next != 0) {
            current_sum = ans.digits[i + count] + for_next;
            ans.digits[i + count] = current_sum % base;
            for_next = current_sum / base;
        }
    }
    while (ans.digits[ans.digits.size() - 1] == 0 && ans.digits.size() != 1) {
        ans.digits.pop_back();
    }
    if (ans.digits[ans.digits.size() - 1] == 0) {
        ans.isNegative = false;
    }
    return ans;

} // LCOV_EXCL_LINE

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (other == BigInt(0)) {
        throw std::invalid_argument("division by zero");
    }
    BigInt div = other;
    BigInt delim = *this;
    BigInt ans;
    ans.digits.pop_back();
    if (!(this->isNegative == other.isNegative)) {
        ans.isNegative = true;
        div.isNegative = false;
        delim.isNegative = false;
    }
    if (this->isNegative && other.isNegative) {
        ans.isNegative = false;
        div.isNegative = false;
        delim.isNegative = false;
    }
    BigInt abs_other = other;
    abs_other.isNegative = false;
    while(1) {
        if (delim < abs_other) {
            break;
        }
        int size_diff = delim.digits.size() - div.digits.size();
        // выравниваем делитель до порядка делимого
        for (int i = 0; i < size_diff; i++) {
            div.digits.insert(div.digits.begin(), 0);
        }
        // пока делитель больше уменьшаем 
        while(div > delim) {
            div.digits.erase(div.digits.begin());
        }
        unsigned long long current_res = 0;
        while (div <= delim) {
            delim -= div;
            current_res++;

        }
        while (delim.digits[delim.digits.size() - 1] == 0) {
            delim.digits.pop_back();
        }
        ans.digits.push_back(current_res);
        div = abs_other;
    }
    reverse(ans.digits.begin(), ans.digits.end());
    if (ans.digits.size() == 0) {
        ans.digits.push_back(0);
    }
    while (ans.digits[ans.digits.size() - 1] == 0 && ans.digits.size() != 1) {
        ans.digits.pop_back();
    }
    if (ans.digits[ans.digits.size() - 1] == 0) {
        ans.isNegative = false;
    }
    return ans;
}

BigInt& BigInt::operator/=(const BigInt& other) {
    *this = *this / other;
    return *this;

}