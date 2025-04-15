
#pragma once

#include <iostream>


template<typename T>
class Container {
public:
    Container() = default;

    Container(const Container &container) = default;

    virtual Container &operator=(const Container &container) = 0;

    virtual ~Container() = default;

    virtual bool operator==(const Container &other) const = 0;
    virtual bool operator!=(const Container &other) const = 0;

    virtual size_t size() const = 0;

    virtual size_t max_size() const = 0;

    virtual bool empty() const = 0;
};