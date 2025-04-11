#ifndef ARRAY_H
#define ARRAY_H

#include "container.hpp"

namespace my_container {
    template<typename T, size_t N>
    class Array final : public Container<T> {
    private:
        T array_data[N]{};

    public:
        using iterator = T *;
        using const_iterator = const T *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        Array() = default;

        Array(std::initializer_list<T> init) {
            if (init.size() != N) {
                throw std::invalid_argument("Invalid size");
            }
            std::copy(init.begin(), init.end(), array_data);
        }

        ~Array() override = default;

        Array(const Array &) = default;

        Array(Array &&) noexcept = default;

        Array &operator=(const Array &other) {
            if (this != &other) {
                std::copy(other.begin(), other.end(), this->begin());
            }
            return *this;
        }

        Array &operator=(Array &&other) noexcept {
            if (this != &other) {
                std::move(other.begin(), other.end(), this->begin());
            }
            return *this;
        }

        Container<T> &operator=(const Container<T> &other) override {
            const Array *array = dynamic_cast<const Array *>(&other);
            if (!array) {
                throw std::invalid_argument("Invalid type");
            }

            std::copy(array->begin(), array->end(), this->begin());
            return *this;
        }

        T &at(size_t position) {
            static_assert(N > 0, "Array is empty");
            if (position >= N)
                throw std::out_of_range("Invalid index");
            return array_data[position];
        }

        const T &at(size_t position) const {
            static_assert(N > 0, "Array is empty");
            if (position >= N)
                throw std::out_of_range("Invalid index");
            return array_data[position];
        }

        T &operator[](size_t position) {
            return array_data[position];
        }

        const T &operator[](size_t position) const {
            return array_data[position];
        }

        T &front() {
            static_assert(N > 0, "Array is empty");
            return at(0);
        }

        const T &front() const {
            static_assert(N > 0, "Array is empty");
            return at(0);
        }

        T &back() {
            static_assert(N > 0, "Array is empty");
            return at(N - 1);
        }

        const T &back() const {
            static_assert(N > 0, "Array is empty");
            return at(N - 1);
        }

        T *data() {
            return array_data;
        }

        const T *data() const {
            return array_data;
        }

        iterator begin() {
            return array_data;
        }

        const_iterator begin() const {
            return array_data;
        }

        const_iterator cbegin() const {
            return array_data;
        }

        iterator end() {
            return array_data + N;
        }

        const_iterator end() const {
            return array_data + N;
        }

        const_iterator cend() const {
            return array_data + N;
        }

        reverse_iterator rbegin() {
            return
                    reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(cend());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        const_reverse_iterator crend() const {
            return const_reverse_iterator(cbegin());
        }

        bool empty() const override {
            return N == 0;
        }

        size_t size() const override {
            return N;
        }

        size_t max_size() const override {
            return N;
        }

        void fill(const T &value) {
            std::fill(begin(), end(), value);
        }

        void swap(Array &other) noexcept {
            std::swap_ranges(begin(), end(), other.begin());
        }

        bool operator==(const Container<T> &other) const override {
            const Array *array = dynamic_cast<const Array *>(&other);
            if (!array)
                return false;

            return std::equal(begin(), end(), array->begin());
        }

        bool operator!=(const Container<T> &other) const override {
            return !(*this == other);
        }

        std::strong_ordering operator<=>(const Array &other) const {
            return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
        }
    };
}

#endif //ARRAY_H
