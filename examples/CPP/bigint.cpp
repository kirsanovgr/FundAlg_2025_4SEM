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

    bigint& operator=(const bigint&) = delete; // просто пример удаления оператора

    friend bigint& operator-(bigint& lhs, const bigint& rhs);
    friend bigint& operator-=(bigint& lhs, const bigint& rhs);

    friend bigint& operator+(const bigint& lhs, const bigint& rhs) {
        bigint res(lhs);
        res += rhs;
        return res;
    }

    bigint& operator+=(const bigint& rhs)
    {
        // Если знаки разные, используем вычитание
        if (is_negative != rhs.is_negative)
        {
            // Копируем right-hand side (правый операнд) чтобы изменить его знак
            bigint temp = rhs;
            temp.is_negative = !temp.is_negative;
            return *this -= temp;
        }

        // Определяем максимальный размер
        size_t max_size = std::max(data.size(), rhs.data.size());
        data.resize(max_size, 0);

        unsigned long long carry = 0;

        for (size_t i = 0; i < max_size; ++i)
        {
            unsigned long long rhs_val = (i < rhs.data.size()) ? rhs.data[i] : 0;
            unsigned long long sum = data[i] + rhs_val + carry;

            // Проверяем на переполнение
            if (sum < data[i] || sum < rhs_val)
            {
                carry = 1;
            }
            else
            {
                carry = sum >> 63; // Для 64-битных чисел (альтернативный способ проверки переполнения)
                if (carry == 0 && (sum >> 63) != 0) carry = 1; // Дополнительная проверка
            }

            data[i] = sum;
        }

        // Если остался перенос
        if (carry != 0)
        {
            data.push_back(carry);
        }

        return *this;
    }
};
