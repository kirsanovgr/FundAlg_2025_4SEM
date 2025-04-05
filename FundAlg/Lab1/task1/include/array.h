#ifndef FUNDAMENTAL_ALGORITHMS_2_ARRAY_H
#define FUNDAMENTAL_ALGORITHMS_2_ARRAY_H

#include <iostream>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

#include "../../container.h"

namespace my_container {
    template<typename T, size_t N>
    class Array final : public Container<T>  {
    private:
        static_assert(N > 0, "Size > 0");

        T _data[N] {};
    public:
        Array() = default;
        Array(const Array&) = default;
        Array(Array&&) noexcept = default;
        Array(std::initializer_list<T> init) {
            if (init.size() != N) {
                throw std::invalid_argument("Invalid size");
            }
            std::copy(init.begin(), init.end(), _data);
        }

        ~Array() override = default;

        Array& operator=(const Array& other) {
            if (this != &other) {
                std::copy(other.begin(), other.end(), this->begin());
            }
            return *this;
        }
        Array& operator=(Array&&) noexcept = default;
        Container<T>& operator=(const Container<T>& other) override {
            const Array *array = dynamic_cast<const Array*>(&other);
            if (!array) {
                throw std::invalid_argument("Invalid type");
            }

            std::copy(array->begin(), array->end(), _data);
            return *this;
        }

        T& at(size_t position) {
            if (position >= N)
                throw std::out_of_range("Index");
            return _data[position];
        }

        const T& at(size_t position) const {
            if (position >= N)
                throw std::out_of_range("Index");
            return _data[position];
        }

        T& operator[](size_t position) { return _data[position]; }

        const T& operator[](size_t position) const { return _data[position]; }

        T& front() { return at(0); }
        const T& front() const { return at(0); }

        T& back() { return at(N - 1); }
        const T& back() const { return at(N - 1); }

        T* data() { return _data; }
        const T* data() const { return _data; }


        using iterator = T*;
        using const_iterator = const T*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        iterator begin() { return _data; }
        const_iterator begin() const { return _data; }
        const_iterator cbegin() const { return _data; }

        iterator end() { return _data + N; }
        const_iterator end() const { return _data + N; }
        const_iterator cend() const { return _data + N; }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }

        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }


        bool empty() const override { return N == 0; }
        size_t size() const override { return N; }
        size_t max_size() const override { return N; }

        void fill(const T& value) {
            std::fill(begin(), end(), value);
        }

        void swap(Array& other) noexcept {
            std::swap_ranges(begin(), end(), other.begin());
        }

        bool operator==(const Container<T>& other) const override {
            const Array* array = dynamic_cast<const Array*>(&other);
            if (!array)
                return false;

            return std::equal(begin(), end(), array->begin());
        }

        bool operator!=(const Container<T>& other) const override {
            return !(*this == other);
        }

        bool operator<(const Array& other) {
            return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }

        std::strong_ordering operator<=>(const Array& other) const noexcept {
            return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
        }
    };
}

#endif //FUNDAMENTAL_ALGORITHMS_2_ARRAY_H
