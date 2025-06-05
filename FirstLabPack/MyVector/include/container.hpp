    #pragma once

#include <iostream>

template <typename T>
class Container {
public:
    Container() = default;
    Container(const Container& other) = default;
    virtual Container<T>& operator=(const Container<T>& other) = 0;
    virtual ~Container() = default;

    virtual bool operator==(const Container& other) const = 0;
    virtual bool operator!=(const Container& other) const = 0;
    virtual T& operator[](size_t index) = 0;
    virtual const T& operator[](size_t index) const = 0;

    virtual size_t size() const = 0;
    virtual size_t capacity() const = 0;
    virtual size_t max_size() const = 0;
    virtual bool empty() const = 0;
};
