#pragma once
#include <initializer_list>
#include <iterator>
#include <compare>
#include "container.h"

namespace my_container {
    
template <typename T, size_t N>
class Array final : public Container<T> {
public:
    Array() = default;
    Array(std::initializer_list<T> initl);
    constexpr size_t max_size() const noexcept override;
    ~Array() = default;
    Array<T,N>& operator=(const Array<T,N>& other);
    constexpr auto operator<=>(const Array<T, N>& rhs) const;
    bool operator==(const Array<T, N>& rhs) const;
    Array(const Array<T, N>& other);
    Array(Array<T,N>&& other) noexcept;
    T* begin() noexcept;
    const T* cbegin() const noexcept;
    T* end() noexcept;
    const T* cend() const noexcept;
    std::reverse_iterator<T*> rbegin() noexcept;
    std::reverse_iterator<const T*> rbegin() const noexcept;
    std::reverse_iterator<const T*> crbegin() const noexcept;
    std::reverse_iterator<T*> rend() noexcept;
    std::reverse_iterator<const T*> rend() const noexcept;
    std::reverse_iterator<const T*> crend() const noexcept;
    T& at(size_t pos);
    const T& at(size_t pos) const;
    T& operator[](size_t pos);
    const T& operator[](size_t pos) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data();
    const T* data() const;
    constexpr bool empty() const noexcept override;
    constexpr size_t size() const noexcept override;
    void fill(const T& value);
    void swap(Array& other) noexcept;

private:
    T mass[N];


};


template <typename T, size_t N>
Array<T,N>::Array(std::initializer_list<T> initl) : mass{} {
    size_t count = 0;
    for (const auto& element : initl) {
        if (count < N) {
            mass[count] = element;
            count++;
        }
        else {
            break;
        }
    }

}


template <typename T, size_t N>
size_t constexpr Array<T,N>::max_size() const noexcept {
    return N;
}

template <typename T, size_t N>
Array<T,N>::Array(const Array<T,N>& other) {
    for (size_t i = 0; i < N; i++) {
        mass[i] = other.mass[i];
    }
}

template <typename T, size_t N>
Array<T,N>& Array<T,N>::operator=(const Array<T,N>& other) {
    for (size_t i = 0; i < N; i++) {
        mass[i] = other.mass[i];
    }
    return *this;
}

template<typename T, size_t N>
T* Array<T,N>::begin() noexcept {
    return mass;
}

template<typename T, size_t N>
const T* Array<T,N>::cbegin() const noexcept {
    return mass;
}

template<typename T, size_t N>
T* Array<T,N>::end() noexcept {
    return mass + N;
}

template<typename T, size_t N>
const T* Array<T,N>::cend() const noexcept {
    return mass + N;
}

template<typename T, size_t N>
std::reverse_iterator<T*> Array<T,N>::rbegin() noexcept {
    return std::reverse_iterator<T*>(end());
}

template<typename T, size_t N>
std::reverse_iterator<const T*> Array<T,N>::rbegin() const noexcept {
    return std::reverse_iterator<const T*>(cend());
}

template<typename T, size_t N>
std::reverse_iterator<const T*> Array<T,N>::crbegin() const noexcept {
    return std::reverse_iterator<const T*>(cend());
}

template<typename T, size_t N>
std::reverse_iterator<T*> Array<T,N>::rend() noexcept {
    return std::reverse_iterator<T*>(begin());
}

template<typename T, size_t N>
std::reverse_iterator<const T*> Array<T,N>::rend() const noexcept {
    return std::reverse_iterator<const T*>(cbegin());
}

template<typename T, size_t N>
std::reverse_iterator<const T*> Array<T,N>::crend() const noexcept {
    return std::reverse_iterator<const T*>(cbegin());
}

template<typename T, size_t N>
T& Array<T,N>::at(size_t pos) {
    if (pos >= N) {
        throw std::out_of_range("Index out of range");
    }
    return mass[pos];
}

template<typename T, size_t N>
const T& Array<T,N>::at(size_t pos) const {
    if (pos >= N) {
        throw std::out_of_range("Index out of range");
    }
    return mass[pos];
}

template<typename T, size_t N>
T& Array<T,N>::operator[](size_t pos) {
    return at(pos);
}

template<typename T, size_t N>
const T& Array<T,N>::operator[](size_t pos) const {
    return at(pos);
}

template<typename T, size_t N>
T& Array<T,N>::front() {
    return mass[0];
}

template<typename T, size_t N>
const T& Array<T,N>::front() const {
    return mass[0];
}

template<typename T, size_t N>
T& Array<T,N>::back() {
    return mass[N - 1];
}

template<typename T, size_t N>
const T& Array<T,N>::back() const {
    return mass[N - 1];
}

template<typename T, size_t N>
const T* Array<T,N>::data() const {
    return mass;
}

template<typename T, size_t N>
T* Array<T,N>::data() {
    return mass;
}


template <typename T, size_t N>
Array<T,N>::Array(Array<T,N>&& other) noexcept {
    std::move(other.begin(), other.end(), this->begin());
}

template<typename T, size_t N>
bool constexpr Array<T,N>::empty() const noexcept {
    return N == 0;
}

template<typename T, size_t N>
size_t constexpr Array<T,N>::size() const noexcept {
    return N;
}

template<typename T, size_t N>
void Array<T,N>::fill(const T& value) {
    for (size_t i = 0; i < N; i++) {
        mass[i] = value;
    }

}

template<typename T, size_t N>
void Array<T,N>::swap(Array& other) noexcept {
    for (size_t i = 0; i < N; i++) {
        T tmp;
        tmp = mass[i];
        mass[i] = other.mass[i];
        other.mass[i] = tmp;
    }
}


template<typename T, size_t N>
constexpr auto Array<T,N>::operator<=>(const Array<T, N>& rhs) const {
    return std::lexicographical_compare_three_way(cbegin(), cend(), rhs.cbegin(), rhs.cend());
}

template<typename T, size_t N >
bool Array<T,N>::operator==(const Array<T, N>& rhs) const {
    return (*this <=> rhs) == std::strong_ordering::equal;
}

}