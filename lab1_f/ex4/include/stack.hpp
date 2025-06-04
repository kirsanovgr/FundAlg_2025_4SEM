#pragma once

#include "deque.hpp"

namespace my_container {
template <typename T>
class Stack final {
   private:
	Deque<T> data;

   public:
	Stack();
	Stack(const Stack<T> &other);
	Stack(Stack<T> &&other) noexcept;
	explicit Stack(const std::initializer_list<T> &init);
	~Stack() = default;

	Stack<T> &operator=(const Stack<T> &other);
	Stack<T> &operator=(Stack<T> &&other) noexcept;

	T &top();
	const T &top() const;
	bool empty() const;
	size_t size() const;

	void push(const T &el);
	void push(T &&el);

	void pop();

	void swap(Stack<T> &other);

	bool operator==(const Stack &other) const;
	bool operator!=(const Stack &other) const;
	bool operator>(const Stack &other) const;
	bool operator>=(const Stack &other) const;
	bool operator<=(const Stack &other) const;
	bool operator<(const Stack &other) const;
	std::strong_ordering operator<=>(const Stack &other) const;
};

template <typename T>
bool Stack<T>::operator!=(const Stack &other) const {
	return (*this <=> other) != std::strong_ordering::equal;
}

template <typename T>
bool Stack<T>::operator>(const Stack &other) const {
	return (*this <=> other) == std::strong_ordering::greater;
}

template <typename T>
bool Stack<T>::operator>=(const Stack &other) const {
	return (*this <=> other) == std::strong_ordering::greater or (*this <=> other) == std::strong_ordering::equal;
}

template <typename T>
bool Stack<T>::operator<(const Stack &other) const {
	return (*this <=> other) == std::strong_ordering::less;
}

template <typename T>
bool Stack<T>::operator<=(const Stack &other) const {
	return (*this <=> other) == std::strong_ordering::less or (*this <=> other) == std::strong_ordering::equal;
}

template <typename T>
const T &Stack<T>::top() const {
	return data.front();
}

template <typename T>
bool Stack<T>::operator==(const Stack &other) const {
	return (*this <=> other) == std::strong_ordering::equal;
}

template <typename T>
std::strong_ordering Stack<T>::operator<=>(const Stack &other) const {
	return data <=> other.data;
}

template <typename T>
void Stack<T>::swap(Stack<T> &other) {
	data.swap(other.data);
}

template <typename T>
void Stack<T>::pop() {
	data.pop_front();
}

template <typename T>
void Stack<T>::push(T &&el) {
	data.push_front(el);
}

template <typename T>
void Stack<T>::push(const T &el) {
	data.push_front(el);
}

template <typename T>
size_t Stack<T>::size() const {
	return data.size();
}

template <typename T>
bool Stack<T>::empty() const {
	return data.empty();
}

template <typename T>
T &Stack<T>::top() {
	return data.front();
}

template <typename T>
Stack<T> &Stack<T>::operator=(Stack<T> &&other) noexcept {
	if (this != &other) {
		data = std::move(other.data);
	}
	return *this;
}

template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &other) {
	if (this != &other) {
		data = other.data;
	}
	return *this;
}

template <typename T>
Stack<T>::Stack(const std::initializer_list<T> &init) : data{init} {}

template <typename T>
Stack<T>::Stack(Stack<T> &&other) noexcept : data{std::move(other.data)} {}

template <typename T>
Stack<T>::Stack(const Stack<T> &other) : data{other.data} {}

template <typename T>
Stack<T>::Stack() : data{} {};

}  // namespace my_container