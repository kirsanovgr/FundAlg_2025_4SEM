#ifndef FUNDAMENTAL_ALGORITHMS_2_CONTAINER_H
#define FUNDAMENTAL_ALGORITHMS_2_CONTAINER_H

#include <iostream>

template<typename T>
class Container {
public:
    Container() = default;
    Container(const Container &c) = default;
    virtual Container& operator=(const Container &c) = 0;
    virtual ~Container() = default;

    virtual bool operator==(const Container &c) const = 0;
    virtual bool operator!=(const Container &c) const = 0;

    virtual size_t size() const = 0;
    virtual size_t max_size() const = 0;
    virtual bool empty() const = 0;
};

#endif //FUNDAMENTAL_ALGORITHMS_2_CONTAINER_H
