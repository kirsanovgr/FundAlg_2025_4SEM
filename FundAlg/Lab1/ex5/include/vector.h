#pragma once
#include <initializer_list>
#include "container.h"

namespace my_container {

template<typename T>
class Vector : public Container<T> {
private:
    size_t size_;
    size_t capacity_;
    T* data_;
public:
    Vector();
    Vector(const Vector<T>& other);
    Vector(Vector<T>&& other);
    Vector(std::initializer_list<T> initl);
    ~Vector();

    Vector<T>& operator=(const Vector<T>& other);

    T& operator[](size_t pos);
    const T& operator[](size_t pos) const;
    T& at(size_t pos);
    const T& at(size_t pos) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data();
    const T* data() const;

    bool empty() const override;
    size_t size() const override;
    size_t capacity() const;
    void reserve(size_t new_cap);
    void shrink_to_fit();

    void clear();
    void push_back(const T& value);
    void pop_back();
    T* insert(const T* pos, const T& value);
    T* erase(const T* pos);
    void resize(size_t count);
    void swap(Vector<T>& other);

    constexpr auto operator<=>(const Vector<T>& rhs) const;
    bool operator==(const Vector<T>& rhs) const;





};

template<typename T>
Vector<T>::Vector() {
    size_ = 0;
    capacity_ = 10;
    data_ = new T[10];

}

template<typename T>
Vector<T>::~Vector() {
    size_ = 0;
    capacity_ = 0;
    delete[] data_;
}

template<typename T>
Vector<T>::Vector(const Vector<T>& other) : size_(other.size_), capacity_(other.capacity_) {
    data_ = new T[capacity_];
    std::copy(other.data_, other.data_ + other.size_, data_);

}

template<typename T>
Vector<T>::Vector(Vector<T>&& other) : size_(other.size_), capacity_(other.capacity_),
                                        data_(other.data_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
    
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> initl) : Vector() {
    reserve(initl.size());
    size_t i = 0;
    for (auto elem : initl) {
        data_[i] = elem;
        i++;
    }
    size_ = initl.size();
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        delete[] data_;
        data_ = new T[capacity_];
        std::copy(other.data_, other.data_ + other.size_, data_);
    }
    return *this;


}

template<typename T>
T& Vector<T>::at(size_t pos) {
    if (pos >= size_) {
        throw std::out_of_range("index is out of range");
    }
    return data_[pos];
}

template<typename T>
const T& Vector<T>::at(size_t pos) const {
    if (pos >= size_) {
        throw std::out_of_range("index is out of range");
    }
    return data_[pos];
}

template<typename T>
T& Vector<T>::operator[](size_t pos) {
    return data_[pos];
}

template<typename T>
const T& Vector<T>::operator[](size_t pos) const {
    return data_[pos];
}

template<typename T>
T& Vector<T>::front() {
    return data_[0];
}

template<typename T>
const T& Vector<T>::front() const {
    return data_[0];
}

template<typename T>
T& Vector<T>::back() {
    return data_[size_ - 1];
}

template<typename T>
const T& Vector<T>::back() const {
    return data_[size_ - 1];
}

template<typename T>
T* Vector<T>::data() {
    return data_;
}

template<typename T>
const T* Vector<T>::data() const {
    return data_;
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
void Vector<T>::reserve(size_t new_cap) {
    if (new_cap > capacity_) {
        capacity_ = new_cap;
        T* new_data_ = new T[capacity_];
        std::copy(data_, data_ + size_, new_data_);
        delete[] data_;
        data_ = new_data_;
    }
}

template<typename T>
void Vector<T>::shrink_to_fit() {
    capacity_ = size_;
    T* new_data_ = new T[capacity_];
    std::copy(data_, data_ + size_, data_);
    delete[] data_;
    data_ = new_data_;
}

template<typename T>
void Vector<T>::clear() {
    size_ = 0;
}

template<typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ + 1 >= capacity_) {
        reserve(capacity_ * 2);
    }
    data_[size_] = value;
    size_++;

} 

template<typename T>
void Vector<T>::pop_back() {
    size_--;
}

template<typename T>
T* Vector<T>::insert(const T* pos, const T& value) {
    size_t position = pos - data_;
    if (size_ + 1 >= capacity_) {
        reserve((capacity_ + 1) * 2);
    }
    for (size_t i = size_; i > position; i--) {
        data_[i] = data_[i - 1];
    }
    data_[position] = value;
    size_++;
    return data_ + position;

}

template<typename T>
T* Vector<T>::erase(const T* pos) {
    size_t position = pos - data_;
    for (size_t i = position; i < size_ - 1; i++) {
        data_[i] = data_[i + 1];
    }
    size_--;
    return data_ + position;

}

template<typename T>
void Vector<T>::resize(size_t count) {
    if (size_ >= count) {
        size_ = count;
    }
    else {
        if (count > capacity_) {
            reserve((capacity_ + 1) * 2);
        }
        for (size_t i = size_; i < count; i++) {
            data_[i] = 0;
        }
        size_ = count;
    }
}

template<typename T>
void Vector<T>::swap(Vector& other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename T>
constexpr auto Vector<T>::operator<=>(const Vector<T>& rhs) const {
    return std::lexicographical_compare_three_way(data_, data_ + size_, rhs.data_, rhs.data_ + rhs.size_);
}

template<typename T>
bool Vector<T>::operator==(const Vector<T>& rhs) const {
    return (*this <=> rhs) == std::strong_ordering::equal;
}



}
