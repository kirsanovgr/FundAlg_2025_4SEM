#pragma once

#include "container.hpp"
#include <cmath>

namespace my_container {

    template<typename T>
    class Vector : public Container<T> {
    private:
        T *data_ = nullptr;
        size_t capacity_ = 0;
        size_t size_ = 0;

    public:
        explicit Vector(size_t capacity = 4);
        Vector(const Vector<T> &other);
        Vector(Vector<T> &&other) noexcept;
        Vector(const std::initializer_list<T> &other);
        Vector<T> &operator=(const Vector<T> &other);
        Vector<T> &operator=(Vector<T> &&other) noexcept;
        ~Vector() override;

        T &operator[](size_t index);
        const T &operator[](size_t index) const;
        T &at(size_t index);
        const T &at(size_t index) const;
        T &front();
        const T &front() const;
        T &back();
        const T &back() const;
        T *data();
        const T *data() const;

        void push_back(const T &el);
        void push_back(T &&el);
        void pop_back();


        T *insert(const T *pos, const T &value);
        T *insert(const T *pos, T &&value);
        T *insert(const T *pos, size_t count, const T &value);
        T * erase(T * pos);
        T * erase(const T * pos);
        T * erase(T * first, T * last);
        T * erase(const T * first, const T * last);
        T *begin();
        T *begin() const;
        const T *cbegin() const;
        T *end();
        T *end() const;
        const T *cend() const;
        void clear();

        size_t capacity() const override;
        size_t size() const override;
        bool empty() const override;
        void shrink_to_fit();
        void resize(size_t new_size, const T &value = 0);
        void reserve(size_t new_capacity);

        template<typename U>
        friend std::ostream &operator<<(std::ostream &out, const Vector<U> &rhs);

        std::strong_ordering operator<=>(const Vector<T> & other) const ;
        bool operator==(const Vector<T> & other) const;
        bool operator>(const Vector<T> & other) const;
        bool operator>=(const Vector<T> & other) const;
        bool operator<(const Vector<T> & other) const;
        bool operator<=(const Vector<T> & other) const;
        void swap(Vector<T> & other);
    };

    template<typename T>
    void Vector<T>::reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            size_t new_capacity_t = std::max(new_capacity, capacity_ * 2);
            T *new_data = new T[new_capacity_t];
            std::copy_n(data_, size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity_t;
        }
    }

    template<typename T>
    void Vector<T>::pop_back() {
        erase(end() - 1);
    }

    template<typename T>
    void Vector<T>::push_back(const T &el) {
        insert(end(), el);
    }

    template<typename T>
    void Vector<T>::push_back(T &&el) {
        insert(end(), el);
    }

    template<typename T>
    bool Vector<T>::operator>(const Vector<T> &other) const {
        return (*this <=> other) == std::strong_ordering::greater;
    }

    template<typename T>
    bool Vector<T>::operator>=(const Vector<T> &other) const {
        return (*this <=> other) == std::strong_ordering::equal or (*this <=> other) == std::strong_ordering::greater;
    }

    template<typename T>
    bool Vector<T>::operator<(const Vector<T> &other) const {
        return (*this <=> other) == std::strong_ordering::less;
    }

    template<typename T>
    bool Vector<T>::operator<=(const Vector<T> &other) const {
        return (*this <=> other) == std::strong_ordering::equal or (*this <=> other) == std::strong_ordering::less;
    }

    template<typename T>
    bool Vector<T>::operator==(const Vector<T> &other) const {
        return (*this <=> other) == std::strong_ordering::equal;
    }

    template<typename T>
    std::strong_ordering Vector<T>::operator<=>(const Vector<T> &other) const{
        return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
    }

    template<typename T>
    T *Vector<T>::erase(T *first, T *last) {
        if(last > end() or first < begin() or first > last) {
            throw std::out_of_range("Erase position is out of range");
        }
        if(first == last) {
            return last;
        }
        size_t count = last - first;
        for(size_t i = 0; i < count;++i) {
            erase(first);
        }
        if(size_ == 0) {
            return end();
        }
        return first;
    }

    template<typename T>
    T *Vector<T>::erase(const T *first, const T *last) {
        if(last > end() or first < begin() or first > last) {
            throw std::out_of_range("Erase position is out of range");
        }
        if(first == last) {
            return last;
        }
        size_t count = last - first;
        for(size_t i = 0; i < count;++i) {
            erase(first);
        }
        if(size_ == 0) {
            return end();
        }
        return first;
    }


    template<typename T>
    T *Vector<T>::erase(T *pos) {
        if(pos > end() or pos < begin()) {
            throw std::out_of_range("Erase position is out of range");
        }
        if(pos == end()) {
            return pos;
        }
        auto it = pos + 1;
        for(; it != end(); ++it) {
            *(it - 1) = *it;
        }
        --size_;

        return it - 1;
    }

    template<typename T>
    T *Vector<T>::erase(const T *pos) {
        if(pos > end() or pos < begin()) {
            throw std::out_of_range("Erase position is out of range");
        }
        if(pos == end()) {
            return pos;
        }
        auto it = pos + 1;
        for(; it != end(); ++it) {
            *(it - 1) = *it;
        }
        --size_;
        return it - 1;
    }

    template<typename T>
    void Vector<T>::swap(Vector<T> &other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    template<typename T>
    T* Vector<T>::insert(const T* pos, size_t count, const T& value) {
        if (pos < data_ || pos > data_ + size_) {
            throw std::out_of_range("Insert position is out of range");
        }
        size_t ind = pos - data_;
        if (size_ + count >= capacity_) {
            size_t ss = size_;
            size_t new_size = int(pow(2, ceil(log2(size_ + count))));
            resize(new_size);
            size_ = ss;
        }
        T * tmp = data_ + ind;
        for (auto it = end() + count; it >= tmp + count; --it) {
            *it = *(it - count);
        }
        for(size_t i = 0; i < count; ++i) {
            data_[i + ind] = value;
        }
        size_ += count;
        return tmp;
    }

    template<typename T>
    T* Vector<T>::insert(const T* pos, const T& value) {
        if (pos < data_ || pos > data_ + size_) {
            throw std::out_of_range("Insert position is out of range");
        }
        size_t ind = pos - data_;
        if (size_ == capacity_) {
            size_t ss = size_;
            resize(capacity_ * 2);
            size_ = ss;
        }
        T * tmp = data_ + ind;
        for (auto it = end(); it != tmp; --it) {
            *it = *(it - 1);
        }
        *tmp = value;
        ++size_;
        return tmp;
    }

    template<typename T>
    T *Vector<T>::insert(const T *pos, T &&value) {
        if (pos < data_ || pos > data_ + size_) {
            throw std::out_of_range("Insert position is out of range");
        }
        size_t ind = pos - data_;
        if (size_ == capacity_) {
            size_t ss = size_;
            resize(capacity_ * 2);
            size_ = ss;
        }
        T * tmp = data_ + ind;
        for (auto it = end(); it != tmp; --it) {
            *it = *(it - 1);
        }
        *tmp = value;
        ++size_;
        return tmp;
    }

    template<typename T>
    void Vector<T>::shrink_to_fit() {
        if (size_ < capacity_) {
            T* new_data = new T[size_];
            std::copy_n(data_, size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = size_;
        }
    }

    template<typename T>
    const T *Vector<T>::data() const {
        return data_;
    }

    template<typename T>
    T *Vector<T>::data() {
        return data_;
    }

    template<typename T>
    T &Vector<T>::front() {
        return *begin();
    }

    template<typename T>
    const T &Vector<T>::front() const {
        return *begin();
    }

    template<typename T>
    T &Vector<T>::back() {
        return *(end() - 1);
    }

    template<typename T>
    const T &Vector<T>::back() const {
        return *(end() - 1);
    }

    template<typename T>
    T &Vector<T>::at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("at");
        }
        return data_[index];
    }

    template<typename T>
    const T &Vector<T>::at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("at");
        }
        return data_[index];
    }

    template<typename T>
    T &Vector<T>::operator[](size_t index) {
        return data_[index];
    }

    template<typename T>
    const T &Vector<T>::operator[](size_t index) const {
        return data_[index];
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(Vector<T> &&other) noexcept {
        if (this != &other) {
            reserve(other.capacity_);
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
            other.clear();
        }
        return *this;
    }

    template<typename T>
    void Vector<T>::clear() {
        size_ = 0;
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
        if (this != &other) {
            resize(other.capacity_);
            std::copy(other.begin(), other.end(), begin());
            size_ = other.size_;
        }
        return *this;
    }

    template<typename T>
    T *Vector<T>::begin() const {
        return data_;
    }

    template<typename T>
    T *Vector<T>::end() const {
        return data_ + size_;
    }

    template<typename T>
    Vector<T>::Vector(const std::initializer_list<T> &other) : Vector(int(pow(2, ceil(log2(other.size()))))) {
        std::copy(other.begin(), other.end(), begin());
        size_ = other.size();
    }

    template<typename T>
    Vector<T>::Vector(Vector<T> &&other) noexcept: data_(other.data_), capacity_(other.capacity_), size_(other.size_) {
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
    }

    template<typename T>
    bool Vector<T>::empty() const {
        return size_ == 0;
    }

    template<typename T>
    size_t Vector<T>::size() const {
        return size_;
    }

    template<typename T>
    size_t Vector<T>::capacity() const {
        return capacity_;
    }

    template<typename T>
    const T *Vector<T>::cbegin() const {
        return data_;
    }

    template<typename T>
    T *Vector<T>::begin() {
        return data_;
    }

    template<typename T>
    const T *Vector<T>::cend() const {
        return data_ + size_;
    }

    template<typename T>
    T *Vector<T>::end() {
        return data_ + size_;
    }

    template<typename T>
    void Vector<T>::resize(size_t new_size, const T &value) {
        if (new_size > capacity_) {
            size_t new_capacity = std::max(new_size, capacity_ * 2);
            T *new_data = new T[new_capacity];
            std::copy_n(data_, size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
        for (size_t i = size_; i < new_size; ++i) {
            data_[i] = value;
        }
        size_ = new_size;
    }

    template<typename T>
    Vector<T>::Vector(const Vector<T> &other) : Vector(other.capacity_) {
        if (&other != this) {
            std::copy(other.begin(), other.end(), begin());
            size_ = other.size();
        }
    }

    template<typename T>
    Vector<T>::~Vector() {
        delete[]data_;
        size_ = 0;
        capacity_ = 0;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const Vector<T> &rhs) {
        for (auto &el: rhs) {
            out << el << ' ';
        }
        out << std::endl;
        return out;
    }

    template<typename T>
    Vector<T>::Vector(size_t capacity) : capacity_{capacity} {
        data_ = new T[capacity];
        size_ = 0;
    }
}