#ifndef FUNDAMENTAL_ALGORITHMS_2_STACK_H
#define FUNDAMENTAL_ALGORITHMS_2_STACK_H

#include "../../task3/include/deque.h"

namespace my_container {

    template<typename T>
    class Stack {
        Deque<T> data;

    public:
        Stack() = default;
        Stack(const Stack& other) : data(other.data) {}
        Stack(Stack&& other) noexcept : data(std::move(other.data)) {}
        Stack(std::initializer_list<T> init) : data(init) {}

        ~Stack() = default;

        Stack& operator=(const Stack &other) {
            data = other.data;
            return *this;
        }

        Stack& operator=(Stack &&other) noexcept {
            data = std::move(other.data);
            return *this;
        }

        T top() { return data.back(); }

        bool empty() { return data.empty(); }
        size_t size() { return data.size(); }

        void push(T element) { data.push_back(element); }
        void pop() { data.pop_back(); }

        void swap(Stack &other) noexcept { data.swap(other.data); }

        bool operator==(const Stack &other) const { return data == other.data; }
        bool operator!=(const Stack &other) const { return data != other.data; }
        bool operator<(const Stack &other) const { return data < other.data; }
        auto operator<=>(const Stack &other) const { return data <=> other.data; }

    };
}

#endif //FUNDAMENTAL_ALGORITHMS_2_STACK_H
