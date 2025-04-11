
#ifndef FUND_ALG_STACK_HPP
#define FUND_ALG_STACK_HPP

#include "deque.hpp"

namespace my_container {

    template <typename T>
    class Stack final  {
    private:
        Deque<T> data;

    public:
        Stack();

        Stack(std::initializer_list<T> init);

        Stack(const Stack<T>& other);

        Stack(Stack<T>&& other) noexcept;

        Stack<T>& operator=(const Stack<T>& other);

        Stack<T>& operator=(Stack<T>&& other) noexcept;

        ~Stack() = default;

        T& top();

        const T& top() const;

        bool empty() const noexcept;

        size_t size() const;

        void push(const T& value);

        void push(T&& value);

        void pop();

        void swap(Stack<T>& other);


        bool operator==(Stack<T> other) const;
        std::strong_ordering operator<=>(Stack<T> other) const;
    };

    template<typename T>
    Stack<T>::Stack(): data{} {};

    template<typename T>
    Stack<T>::Stack(std::initializer_list<T> init): data{init} {};

    template<typename T>
    Stack<T>::Stack(const Stack<T> &other): data(other.data) {};

    template<typename T>
    Stack<T>::Stack(Stack<T> &&other) noexcept: data(std::move(other.data)) {} ;

    template<typename T>
    Stack<T> &Stack<T>::operator=(const Stack<T> &other) {
        if (*this != other) {
            data = other.data;
        }
        return *this;
    }

    template<typename T>
    Stack<T> &Stack<T>::operator=(Stack<T> &&other) noexcept {
        if (*this != other)  {
            data = std::move(other.data);
        }
        return *this;
    }

    template<typename T>
    T &Stack<T>::top() {
        return data.front();
    }

    template<typename T>
    const T& Stack<T>::top() const {
        return data.front();
    }


    template<typename T>
    bool Stack<T>::empty() const noexcept{
        return data.empty();
    }

    template<typename T>
    size_t Stack<T>::size() const {
        return data.size();
    }

    template<typename T>
    void Stack<T>::push(const T &value) {
        data.push_front(value);
    }


    template<typename T>
    void Stack<T>::push(T &&value) {
        data.push_front(value);
    }

    template<typename T>
    void Stack<T>::pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        data.pop_front();
    }

    template<typename T>
    bool Stack<T>::operator==(const Stack<T> other) const {
        return (*this <=> other) == std::strong_ordering::equal;
    }

    template<typename T>
    std::strong_ordering Stack<T>::operator<=>(const Stack<T> other) const {
        return data <=> other.data;
    }


    template<typename T>
    void Stack<T>::swap(Stack<T> &other) {
        data.swap(other.data);
    }
}

#endif //FUND_ALG_STACK_HPP
