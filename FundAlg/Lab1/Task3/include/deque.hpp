#ifndef DEQUE_H
#define DEQUE_H

#include "container.hpp"
#include "list.hpp"

namespace my_container {
    template<typename T>
    class Deque : public List<T> {
    public:
        using List<T>::operator=;

        Deque() = default;

        Deque(std::initializer_list<T> init) : List<T>(init) {
        }

        explicit Deque(const List<T> &other) : List<T>(other) {
        }

        explicit Deque(List<T> &&other) noexcept : List<T>(std::move(other)) {
        }

        List<T> &operator=(const List<T> &other) override {
            List<T>::operator=(other);
            return *this;
        }

        List<T> &operator=(List<T> &&other) noexcept override {
            List<T>::operator=(std::move(other));
            return *this;
        }

        T &at(size_t pos) {
            if (pos >= this->size()) {
                throw std::out_of_range("Index is out of range");
            }
            auto it = this->begin();
            std::advance(it, pos);
            return *it;
        }

        const T &at(size_t pos) const {
            if (pos >= this->size()) {
                throw std::out_of_range("Index is out of range");
            }
            auto it = this->cbegin();
            std::advance(it, pos);
            return *it;
        }

        T &operator[](size_t pos) { return at(pos); }
        const T &operator[](size_t pos) const { return at(pos); }
    };
}

#endif //DEQUE_H
