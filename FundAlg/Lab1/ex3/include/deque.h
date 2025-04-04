#pragma once

#include "list.h"

namespace my_container {

template<typename T>
class Deque : public List<T> {
public:
    Deque() : List<T>() {};
    Deque(const Deque<T>& other) : List<T>(other) {};
    Deque(Deque<T>&& other) noexcept : List<T>(std::move(other)) {};
    Deque(std::initializer_list<T> initl) : List<T>(initl) {};
    Deque<T>& operator=(const Deque<T>& other);

    T &at(size_t pos);
	T &operator[](size_t pos);

};

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& other) {
    if (*this != other) {
        List<T>::operator=(other);
    }
    return *this;
}

template <typename T>
T &Deque<T>::operator[](size_t pos) {
	auto iter = this->begin();
	for (size_t i = 0; i < pos; i++) {
		iter++;
	}
	return *iter;
}

template <typename T>
T &Deque<T>::at(size_t pos) {
	if (pos >= this->size()) {
		throw std::out_of_range("pos is out of range");
	}
	return (*this)[pos];
}

}