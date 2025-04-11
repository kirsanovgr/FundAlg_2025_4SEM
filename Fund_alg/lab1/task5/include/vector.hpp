#pragma once
#include "container.hpp"
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <memory>
#include <cmath>

namespace my_container {

    template <typename T>
    class Vector : public Container<T> {
    public:
        using iterator = T*;
        using const_iterator = const T*;

        Vector();

        Vector(const std::initializer_list<T>& other);

        Vector(const Vector& other);

        Vector(Vector&& other) noexcept;

        ~Vector() override;

        Vector& operator=(const Vector& other);

        Vector& operator=(Vector&& other) noexcept;

        Vector& operator=(std::initializer_list<T> ilist);

        Container<T>& operator=(const Container<T>& other) override;

        bool operator==(const Container<T>& other) const override;

        bool operator!=(const Container<T>& other) const override;

        std::strong_ordering operator<=>(const Vector& other) const;

        T& operator[](size_t i);
        const T& operator[](size_t i) const;

        T& at(size_t i);

        const T& at(size_t i) const;

        T& front();

        const T& front() const;

        T& back();

        const T& back() const;

        T* data() noexcept;
        const T* data() const noexcept;

        iterator begin() noexcept { return data_; }
        iterator end() noexcept { return data_ + size_; }
        const_iterator begin() const noexcept { return data_; }
        const_iterator end() const noexcept { return data_ + size_; }
        const_iterator cbegin() const noexcept { return data_; }
        const_iterator cend() const noexcept { return data_ + size_; }

        size_t size() const override { return size_; }
        size_t max_size() const override {
            return std::numeric_limits<size_t>::max() / sizeof(T);
        }
        bool empty() const override { return size_ == 0; }
        size_t capacity() const { return capacity_; }

        void push_back(const T& value);

        void push_back(T&& value);

        void pop_back();

        void clear() noexcept;

        void reserve(size_t new_capacity);

        void shrink_to_fit();

        iterator insert(const_iterator pos, const T& value);

        iterator insert(const_iterator pos, size_t count, const T& value);
        iterator insert(const_iterator pos, T&& value);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        void resize(size_t new_size, const T& value = T());

        void swap(Vector& other) noexcept;

    private:
        T* data_ = nullptr;
        size_t size_ = 0;
        size_t capacity_ = 0;

    };

    template<typename T>
    Vector<T>::Vector() {
        capacity_ = 2;
        size_ = 0;
        data_ = new T[capacity_];
    }

    template<typename T>
    void Vector<T>::swap(Vector &other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    template<typename T>
    void Vector<T>::resize(size_t new_size, const T &value) {
        if (new_size < size_) {
            while (size_ > new_size) pop_back();
        } else {
            reserve(new_size);
            for (size_t i = size_; i < new_size; ++i) {
                data_[i] = value;
            }
            size_ = new_size;
        }
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::erase(Vector::const_iterator first, Vector::const_iterator last) {
        if (first >= last) return begin();

        const size_t start = first - begin();
        const size_t count = last - first;

        for (size_t i = start; i < size_ - count; ++i) {
            data_[i] = std::move(data_[i + count]);
        }

        for (size_t i = size_ - count; i < size_; ++i) {
            data_[i].~T();
        }

        size_ -= count;
        return begin() + start;
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::erase(Vector::const_iterator pos) {
        return erase(pos, pos + 1);
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::insert(Vector::const_iterator pos, size_t count, const T& value) {
        if (pos < begin() || pos > end()) {
            throw std::out_of_range("Insert position is out of range");
        }
        size_t ind = pos - begin();
        if (size_ + count > capacity_) {
            size_t new_capacity = std::max(capacity_ * 2, size_ + count);
            reserve(new_capacity);
        }
        T* tmp = data_ + ind;
        for (auto it = end() + count - 1; it >= tmp + count; --it) {
            *it = std::move(*(it - count));
        }
        for (size_t i = 0; i < count; ++i) {
            new(tmp + i) T(value);
        }
        size_ += count;
        return tmp;
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::insert(Vector::const_iterator pos, const T& value) {
        return insert(pos, 1, value);
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::insert(Vector::const_iterator pos, T&& value) {
        if (pos < begin() || pos > end()) {
            throw std::out_of_range("Insert position is out of range");
        }
        size_t ind = pos - begin();
        if (size_ >= capacity_) {
            reserve(capacity_ * 2);
        }
        T* tmp = data_ + ind;
        for (auto it = end(); it > tmp; --it) {
            *it = std::move(*(it - 1));
        }
        new(tmp) T(std::move(value));
        ++size_;
        return tmp;
    }

    template<typename T>
    void Vector<T>::clear() noexcept {
        while (size_ > 0) pop_back();
    }

    template<typename T>
    void Vector<T>::pop_back() {
        if (size_ > 0) {
            data_[--size_].~T();
        }
    }

    template<typename T>
    void Vector<T>::shrink_to_fit() {
        if (capacity_ > size_) {
            T* new_data = new T[size_];
            std::copy(data_, data_ + size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = size_;
        }
    }

    template<typename T>
    void Vector<T>::reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = new T[new_capacity];
            std::copy(data_, data_ + size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    template<typename T>
    void Vector<T>::push_back(const T &value) {
        insert(end(), value);
    }

    template<typename T>
    void Vector<T>::push_back(T &&value) {
        insert(end(), std::move(value));
    }

    template<typename T>
    const T *Vector<T>::data() const noexcept { return data_; }

    template<typename T>
    T *Vector<T>::data() noexcept { return data_; }

    template<typename T>
    const T &Vector<T>::back() const {
        if (empty()) throw std::out_of_range("Vector is empty");
        return data_[size_-1];
    }

    template<typename T>
    T &Vector<T>::back() {
        if (empty()) throw std::out_of_range("Vector is empty");
        return data_[size_-1];
    }

    template<typename T>
    const T &Vector<T>::front() const {
        if (empty()) throw std::out_of_range("Vector is empty");
        return data_[0];
    }

    template<typename T>
    T &Vector<T>::front() {
        if (empty()) throw std::out_of_range("Vector is empty");
        return data_[0];
    }

    template<typename T>
    const T &Vector<T>::at(size_t i) const {
        if (i >= size_) throw std::out_of_range("Vector index out of range");
        return data_[i];
    }

    template<typename T>
    T &Vector<T>::at(size_t i) {
        if (i >= size_) throw std::out_of_range("Vector index out of range");
        return data_[i];
    }

    template<typename T>
    const T &Vector<T>::operator[](size_t i) const { return data_[i]; }

    template<typename T>
    T &Vector<T>::operator[](size_t i) { return data_[i]; }

    template<typename T>
    std::strong_ordering Vector<T>::operator<=>(const Vector &other) const {
        return std::lexicographical_compare_three_way(
                begin(), end(), other.begin(), other.end());
    }

    template<typename T>
    bool Vector<T>::operator!=(const Container<T> &other) const {
        return !(*this == other);
    }

    template<typename T>
    bool Vector<T>::operator==(const Container<T> &other) const {
        const Vector* p = dynamic_cast<const Vector*>(&other);
        if (!p || size_ != p->size_) return false;
        return std::equal(begin(), end(), p->begin());
    }

    template<typename T>
    Container<T> &Vector<T>::operator=(const Container<T> &other) {
        if (const Vector* p = dynamic_cast<const Vector*>(&other))
            return *this = *p;
        throw std::bad_cast();
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(std::initializer_list<T> ilist) {
        Vector tmp(ilist);
        swap(tmp);
        return *this;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            delete[] data_;

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    template<typename T>
    Vector<T>::~Vector() {
        clear();
        delete[] data_;
    }

    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept
            : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    template<typename T>
    Vector<T>::Vector(const Vector<T> &other)
            : size_(other.size_), capacity_(other.size_)
    {
        data_ = new T[capacity_];
        std::copy(other.data_, other.data_ + size_, data_);
    }

    template<typename T>
    Vector<T>::Vector(const std::initializer_list<T> &other) : Vector() {
        reserve(other.size());
        for (const T& el : other) {
            push_back(el);
        }
    }

} // namespace my_container