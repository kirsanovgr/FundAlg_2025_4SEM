//
// Created by Олеся on 11.05.2025.
//

#ifndef STACK_STACK_HPP
#define STACK_STACK_HPP

#include "deque.hpp"

template <typename T>
class Stack final: public Deque<T> {
    Deque<T> data;
public:
    Stack();
    Stack(const Stack<T>& other);
    Stack(Stack<T> &&other) noexcept;
    Stack(std::initializer_list<T> init);

    ~Stack() = default;

    Stack<T> & operator=(const Stack<T> & other);
    Stack<T> & operator=(Stack<T> && other) noexcept;

    T& top();
    const T& top() const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void push(const T& val);
    void push(T&& val);
    void pop();
    void swap(Stack<T> & other);

    bool operator==(Stack<T> other) const;
    std::strong_ordering operator<=>(Stack<T> other) const;
};

    template <typename T>
    Stack<T> :: Stack(): data{} {};

    template <typename T>
    Stack<T>::Stack(const Stack<T> &other)
            : Deque<T>(other), data(other.data) {}

    template <typename T>
    Stack<T> ::Stack(Stack<T> &&other) noexcept: data(std::move(other.data)) {};

    template <typename T>
    Stack<T>::Stack(std::initializer_list<T> init): data{init} {};

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
    size_t Stack<T>::size() const noexcept{
        return data.size();
    }

    template<typename T>
    bool Stack<T>::empty() const noexcept{
        return data.empty();
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
#endif //STACK_STACK_HPP
