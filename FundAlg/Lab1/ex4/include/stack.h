#pragma once
#include <initializer_list>
#include "deque.h"

namespace my_container {

template<typename T>
class Stack final {
private:
    Deque<T> data;
public:
    Stack() : data() {};
    Stack(const Stack<T>& other) : data(other.data) {};
    Stack(std::initializer_list<T> initl) : data(initl) {};
    Stack(Stack<T>&& other) noexcept : data(std::move(other.data)) {};
    // деструктор только data
    ~Stack() = default;
    Stack<T>& operator=(const Stack<T>& other);

    T& top();
    const T& top() const;

    bool empty() const;
    size_t size() const;

    void push(const T& value);
    void pop();

    void swap(Stack<T>& other);

    std::strong_ordering operator<=>(const Stack<T>& rhs) const;
    bool operator==(const Stack<T>& rhs) const;



};

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other) {
    if (*this != other) {
        data = other.data;
    }
    return *this;
}

template<typename T>
T& Stack<T>::top() {
    return data.front();
}

template<typename T>
const T& Stack<T>::top() const {
    return data.front();
}

template<typename T>
bool Stack<T>::empty() const {
    return data.empty();
}

template<typename T>
size_t Stack<T>::size() const {
    return data.size();
}

template<typename T>
void Stack<T>::push(const T& value) {
    data.push_front(value);
}

template<typename T>
void Stack<T>::pop() {
    data.pop_front();
}

template<typename T>
void Stack<T>::swap(Stack<T>& other) {
    data.swap(other.data);
}

template<typename T>
std::strong_ordering Stack<T>::operator<=>(const Stack<T>& rhs) const {
    return data <=> rhs.data;
}

template<typename T>
bool Stack<T>::operator==(const Stack<T>& rhs) const {
    return (*this <=> rhs) == std::strong_ordering::equal;
}


}