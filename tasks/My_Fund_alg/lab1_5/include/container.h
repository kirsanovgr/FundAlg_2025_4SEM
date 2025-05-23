#pragma once

#include <vector>
#include <iostream>

template <typename T>
class Container {
public:
    Container() = default;
    Container(const Container& other) = default;
    Container& operator=(const Container& other) = default;

    virtual ~Container() = default;

//    virtual T* begin() = 0;
//    virtual const T* cbegin() const = 0;
//    virtual T* end() = 0;
//    virtual const T* cend() const = 0;

    bool operator==(const Container& other) const = default;
    bool operator!=(const Container& other) const = default;

    virtual size_t size() const = 0;
    virtual size_t capacity() const = 0;
    virtual bool empty() const = 0;
};
