
#pragma once

#include <iostream>


template<typename T>
class Container {
public:
    Container() = default;

    Container(const Container &container) = default;

    virtual Container &operator=(const Container &container) = 0;

    virtual ~Container() = default;
    BigInt::BigInt(long long value, unsigned int base) : base(base) {
        if (base % 10 != 0) {
            throw std::invalid_argument("base must be power of 10\n");
        }
        isNegative = (value < 0);


        unsigned long long abs_value = std::abs(value);
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
    virtual bool operator==(const Container &other) const = 0;
    virtual bool operator!=(const Container &other) const = 0;

    virtual size_t size() const = 0;

    virtual size_t max_size() const = 0;

    virtual bool empty() const = 0;
};