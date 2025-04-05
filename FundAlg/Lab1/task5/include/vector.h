#ifndef FUNDAMENTAL_ALGORITHMS_2_VECTOR_H
#define FUNDAMENTAL_ALGORITHMS_2_VECTOR_H

#include "../../container.h"

namespace my_container {

    template<typename T>
    class Vector : public Container<T> {
    private:
        T* _data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;

        void reallocate(size_t new_capacity) {
            T* new_data = new T[new_capacity];
            std::move(_data, _data + _size, new_data);
            delete[] _data;
            _data = new_data;
            _capacity = new_capacity;
        }
    public:
        Vector() = default;

        Vector(std::initializer_list<T> init) : _size(init.size()), _capacity(init.size() * 2) {
            _data = new T[_capacity];
            std::copy(init.begin(), init.end(), _data);
        }

        Vector(const Vector& other) : _size(other._size), _capacity(other._size) {
            _data = new T[_capacity];
            std::copy(other._data, other._data + _size, _data);
        }

        Vector(Vector&& other) noexcept
                : _data(other._data), _size(other._size), _capacity(other._capacity) {
            other._data = nullptr;
            other._size = 0;
            other._capacity = 0;
        }

        ~Vector() override { delete[] _data; }

        Vector& operator=(const Vector& other) {
            if (this != &other) {
                delete[] _data;
                _size = other._size;
                _capacity = other._size;
                _data = new T[_capacity];
                std::copy(other._data, other._data + _size, _data);
            }
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            if (this != &other) {
                delete[] _data;
                _data = other._data;
                _size = other._size;
                _capacity = other._capacity;
                other._data = nullptr;
                other._size = 0;
                other._capacity = 0;
            }
            return *this;
        }

        Container<T>& operator=(const Container<T>& other) override {
            const Vector* vec = dynamic_cast<const Vector*>(&other);

            *this = *vec;
            return *this;
        }

        T& operator[](size_t pos) { return _data[pos]; }
        const T& operator[](size_t pos) const { return _data[pos]; }

        T& at(size_t pos) {
            if (pos >= _size) throw std::out_of_range("Vector");
            return _data[pos];
        }

        const T& at(size_t pos) const {
            if (pos >= _size) throw std::out_of_range("Vector");
            return _data[pos];
        }

        T& front() {
            return _data[0];
        }

        const T& front() const {
            return _data[0];
        }

        T& back() {
            return _data[_size - 1];
        }

        const T& back() const {
            return _data[_size - 1];
        }

        T* data() noexcept { return _data; }
        const T* data() const noexcept { return _data; }

        bool empty() const override { return _size == 0; }
        size_t size() const override { return _size; }
        size_t capacity() const { return _capacity; }
        size_t max_size() const override { return capacity(); }

        void reserve(size_t new_capacity) {
            if (new_capacity > _capacity) {
                reallocate(new_capacity);
            }
        }

        void shrink_to_fit() {
            if (_capacity > _size) {
                reallocate(_size);
            }
        }

        void clear() {
            delete[] _data;
            _data = nullptr;
            _size = 0;
            _capacity = 0;
        }

        void push_back(const T& value) {
            if (_size >= _capacity) {
                reserve(_capacity == 0 ? 1 : _capacity * 2);
            }
            _data[_size++] = value;
        }

        void pop_back() {
            --_size;
        }

        void insert(size_t pos, const T& value) {
            if (pos > _size)
                throw std::out_of_range("Vector");

            if (_size + 1 > _capacity)
                reserve(_capacity * 2);
            std::move_backward(_data + pos, _data + _size, _data + _size + 1);
            _data[pos] = value;
            ++_size;
        }

        void erase(size_t pos) {
            if (pos >= _size)
                throw std::out_of_range("Vector");

            std::move(_data + pos + 1, _data + _size, _data + pos);
            --_size;
        }

        void resize(size_t count, const T& value = T()) {
            if (count > _capacity)
                reserve(count);
            if (count > _size)
                std::fill(_data + _size, _data + count, value);

            _size = count;
        }

        void swap(Vector& other) noexcept {
            std::swap(_data, other._data);
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
        }

        bool operator==(const Vector& other) const {
            return _size == other._size && std::equal(_data, _data + _size, other._data);
        }

        bool operator!=(const Vector& other) const { return !(*this == other); }

        bool operator==(const Container<T>& other) const override {
            const Vector* vec = dynamic_cast<const Vector*>(&other);
            return vec && this->_size == vec->_size && std::equal(_data, _data + _size, vec->_data);
        }

        bool operator!=(const Container<T>& other) const override {
            return !(*this == other);
        }

        bool operator<(const Vector& other) const {
            return std::lexicographical_compare(_data, _data + _size, other._data, other._data + other._size);
        }

        auto operator<=>(const Vector& other) const {
            return std::lexicographical_compare_three_way(_data, _data + _size, other._data, other._data + other._size);
        }
    };
}

#endif //FUNDAMENTAL_ALGORITHMS_2_VECTOR_H
