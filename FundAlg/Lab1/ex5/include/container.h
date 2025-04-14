#pragma once
#include <iostream>

template <typename T>
class Container {
public:
    Container() = default;
    Container(const Container<T>& other) = default;
    Container<T>& operator=(const Container<T>& other) = default;
    virtual ~Container() = default;
    bool operator==(const Container& other) const = default;
    bool operator!=(const Container& other) const = default;
    virtual size_t size() const = 0;
    virtual bool empty() const  = 0;

};