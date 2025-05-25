//
// Created by senya on 13.05.2025.
//

#pragma once

#include "list.h"
#include <initializer_list>
#include <iterator>
#include <compare>

namespace my_container {

    template <typename T>
    class Deque : public List<T> {
    public:
        using typename List<T>::Node;
        using typename List<T>::iterator;
        using typename List<T>::const_iterator;
        using List<T>::operator=;
        using List<T>::operator==;
        Deque() = default;
        Deque(const Deque& other) = default;
        Deque(Deque&& other) noexcept = default;
        Deque(std::initializer_list<T> init) {
            for (const auto& item : init) {
                this->push_back(item);
            }
        }
        ~Deque() override = default;

        Deque& operator=(const Deque& other) = default;
        Deque& operator=(Deque&& other) noexcept = default;

        T& at(size_t pos) {
            if (pos >= this->size()) throw std::out_of_range("Deque::at: out of range");
            // a
            return *std::next(this->begin(), pos);
        }

        const T& at(size_t pos) const {
            if (pos >= this->size()) throw std::out_of_range("Deque::at: out of range");
            //b
            return *std::next(this->cbegin(), pos);
        }

        T& operator[](size_t pos) { return *std::next(this->begin(), pos); }
        const T& operator[](size_t pos) const { return *std::next(this->cbegin(), pos); }

        T& front() override { return *this->begin(); }
        const T& front() const override { return *this->cbegin(); }

        T& back() override { return *std::prev(this->end()); }
        const T& back() const override { return *std::prev(this->cend()); }

        iterator begin() noexcept override { return List<T>::begin(); }
        const_iterator cbegin() const noexcept override { return List<T>::cbegin(); }

        iterator end() noexcept override { return List<T>::end(); }
        const_iterator cend() const noexcept override { return List<T>::cend(); }
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        reverse_iterator rbegin() noexcept override { return std::reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept override { return std::reverse_iterator(cend()); }

        reverse_iterator rend() noexcept override { return std::reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept override { return std::reverse_iterator(cbegin()); }

        bool empty() const override { return List<T>::empty(); }
        size_t size() const override { return List<T>::size(); }
        size_t max_size() const noexcept override{ return List<T>::max_size(); }
        void clear() override { List<T>::clear(); }

        iterator insert(iterator pos, const T& value) {
            return List<T>::insert(pos, value);
        }

        iterator erase(iterator pos) {
            return List<T>::erase(pos);
        }

        void push_back (const T& value) override { List<T>::push_back(value); }
        void pop_back() override { List<T>::pop_back(); }

        void push_front(const T& value) override { List<T>::push_front(value); }
        void pop_front() override { List<T>::pop_front(); }

        void resize(size_t count) { List<T>::resize(count); }
        void resize(size_t count, const T& value) override {
            if (count < this->size()) {
                List<T>::resize(count); // уменьшение
            } else {
                size_t diff = count - this->size();
                for (size_t i = 0; i < diff; ++i) {
                    this->push_back(value);
                }
            }
        }
        void swap(Deque& other) { List<T>::swap(other); }

        bool operator==(const Deque& other) const {
            return std::equal(this->cbegin(), this->cend(), other.cbegin(), other.cend());
        }

        auto operator<=>(const Deque& other) const {
            return std::lexicographical_compare_three_way(this->cbegin(), this->cend(),
                                                          other.cbegin(), other.cend());
        }
    };

} // namespace my_container
