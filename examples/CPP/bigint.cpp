#include <cmath>
#include <vector>
#include <string>
#include <
const unsigned long long MAX_NUM = 10000000000000;
class bigint
{
private:
    std::vector<unsigned long long> data{};
    bool is_negative = 0;
public:
    bigint() = default;
    ~bigint() = default;
    bigint(long long num) {
        if (num < 0) {
            is_negative = 1;
            num *= -1;
        }

        while (num > 0) {
            data.push_back(num % MAX_NUM);
            num /= MAX_NUM;
        }
    }

    bigint (const std::string& num) {
        size_t base_len = log10(MAX_NUM);
        std::string tmp_str;
        // 1 2 | 3 4 | 5 6 | 7 8
        // base_len = 2
        for (int i = num.size() - 1; i >= 0; i--) {
            if (tmp_str.size() >= base_len) {
                data.push_back(atoll(tmp_str.c_str()));
                tmp_str.erase();
            }
            tmp_str = num[i] + tmp_str;
        }
        data.push_back(atoll(tmp_str.c_str()));
        tmp_str.erase();
    }

    bigint& operator=(const bigint&) = delete;

    friend bigint& operator-(bigint& lhs, const bigint& rhs);

    friend bigint& operator+(const bigint& lhs, const bigint& rhs) {
        bigint res(lhs);
        res += rhs;
        return res;
    }

    bigint& operator+=(const bigint& rhs) {
        int carry = 0;
        unsigned long long first = 0;
        unsigned long long second = 0;
        long long carry = 0;
        std::vector<unsigned long long> res{};

        if (!(rhs.is_negative ^ is_negative)) {
            for (int i = 0; i < rhs.data.size() || i < data.size(); i++) {
                if (i >= rhs.data.size()) {
                    second = 0;
                    first = data[i];
                } else if (i >= data.size()) {
                    first = 0;
                    second = rhs.data[i];
                } else {
                    second = rhs.data[i];
                    first = data[i];
                }
                unsigned long long tmp_res = first + second + carry;
                res.push_back(tmp_res % MAX_NUM);
                carry = (first + second + carry) / MAX_NUM;
            }
        } else if (rhs.negative) {
            *this -= rhs;
        } else {
            rhs -= *this;
        }

        return * this;
    }

};
