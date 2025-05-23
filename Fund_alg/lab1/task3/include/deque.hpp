#pragma once
#include "List.hpp"

namespace my_container {

    template<typename T>
    class Deque : public List<T> {
    public:
        Deque() = default;

        Deque(std::initializer_list<T> init) : List<T>(init) {}

        Deque(const Deque<T> &other);

        Deque(Deque<T> && other) noexcept;

        Deque<T> &operator=(const Deque<T> &other);

        Deque<T> &operator=(Deque<T> &&other) noexcept;

        T &at(size_t pos);

        const T &at(size_t pos) const;

        T &operator[](size_t pos);

        const T &operator[](size_t pos) const;


        bool operator==(const Container<T>& other) const override;
        bool operator!=(const Container<T>& other) const override;
    };

    template<typename T>
    bool Deque<T>::operator==(const Container<T>& other) const {
        return static_cast<const List<T>&>(*this) == static_cast<const List<T>&>(other);
    }

    template<typename T>
    bool Deque<T>::operator!=(const Container<T>& other) const {
        return !(*this == other);
    }

    template<typename T>
    Deque<T> &Deque<T>::operator=(Deque<T> &&other) noexcept {
        List<T>::operator=(std::move(other));
        return *this;
    }

    template<typename T>
    Deque<T> &Deque<T>::operator=(const Deque<T> &other) {
        List<T>::operator=(other);
        return *this;
    }

    template<typename T>
    Deque<T>:: Deque(Deque<T> &&other) noexcept : List<T>(std::move(other)) {}

    template<typename T>
    Deque<T>::Deque(const Deque<T> &other) : List<T>(other) {}


    template <typename T>
    T& Deque<T>::operator[](size_t pos) {
        if (pos >= this->size()) {
            throw std::out_of_range("Out of range");
        }
        auto it = this->begin();
        std::advance(it, pos);
        return *it;
    }

    template <typename T>
    T& Deque<T>::at(size_t pos) {
        if (pos >= this->size()) {
            throw std::out_of_range("Out of range");
        }
        // nc
        return (*this)[pos];
    }

    template<typename T>
    const T& Deque<T>::operator[](size_t pos) const {
        if (pos >= this->size()) {
            throw std::out_of_range("Out of range");
        }
        auto it = this->cbegin();
        std::advance(it, pos);
        return *it;
    }

    template<typename T>
    const T& Deque<T>::at(size_t pos) const {
        if (pos >= this->size()) {
            throw std::out_of_range("Out of range");
        }
        // c
        return (*this)[pos];
    }
}

