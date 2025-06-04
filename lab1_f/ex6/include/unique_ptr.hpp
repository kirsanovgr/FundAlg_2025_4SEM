#pragma once

#include <iostream>

namespace my_smart_ptr {
template <class T>
class UniquePtr {
   private:
	T *pointer = nullptr;

   public:
	constexpr UniquePtr() noexcept = default;
	explicit UniquePtr(T *ptr) noexcept;
	UniquePtr(const UniquePtr &other) = delete;
	UniquePtr(UniquePtr &&other) noexcept;

	UniquePtr &operator=(UniquePtr &&other) noexcept;
	UniquePtr &operator=(const UniquePtr &other) noexcept = delete;

	T *get() const noexcept;
	T &operator*() const;
	T *operator->() const noexcept;
	explicit operator bool() const noexcept;

	void reset(T *ptr = nullptr) noexcept;
	T *release() noexcept;
	void swap(UniquePtr &other) noexcept;
	~UniquePtr();
};

template <typename T>
class UniquePtr<T[]> {
	T *pointer = nullptr;

   public:
	constexpr UniquePtr() noexcept = default;
	explicit UniquePtr(T *ptr) noexcept;
	UniquePtr(UniquePtr &&other) noexcept;

	UniquePtr(const UniquePtr &) = delete;
	UniquePtr &operator=(const UniquePtr &) = delete;
	UniquePtr &operator=(UniquePtr &&other) noexcept;
	~UniquePtr();
	T *get() const noexcept;
	T &operator*() const { return *pointer; }
	T *operator->() const noexcept { return pointer; }
	explicit operator bool() const noexcept { return pointer != nullptr; }
	T &operator[](size_t index) const;

	void reset(T *ptr = nullptr) noexcept;
	T *release() noexcept;
	void swap(UniquePtr &other) noexcept;
};

template <typename T>
void UniquePtr<T[]>::swap(UniquePtr &other) noexcept {
	std::swap(pointer, other.pointer);
}

template <typename T>
T *UniquePtr<T[]>::release() noexcept {
	T *ptr = pointer;
	pointer = nullptr;
	return ptr;
}

template <typename T>
void UniquePtr<T[]>::reset(T *ptr) noexcept {
	delete[] pointer;
	pointer = ptr;
}

template <typename T>
T &UniquePtr<T[]>::operator[](size_t index) const {
	return pointer[index];
}

template <typename T>
T *UniquePtr<T[]>::get() const noexcept {
	return pointer;
}

template <typename T>
UniquePtr<T[]>::~UniquePtr() {
	delete[] pointer;
}

template <typename T>
UniquePtr<T[]> &UniquePtr<T[]>::operator=(UniquePtr &&other) noexcept {
	reset(other.release());
	return *this;
}

template <typename T>
UniquePtr<T[]>::UniquePtr(UniquePtr &&other) noexcept : pointer(other.release()) {}

template <typename T>
UniquePtr<T[]>::UniquePtr(T *ptr) noexcept : pointer(ptr) {}

template <class T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
	this->pointer = other.release();
	return *this;
}

template <class T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept {
	pointer = other.release();
}

template <class T>
void UniquePtr<T>::swap(UniquePtr &other) noexcept {
	std::swap(pointer, other.pointer);
}

template <class T>
T *UniquePtr<T>::release() noexcept {
	T *tmp = pointer;
	pointer = nullptr;
	return tmp;
}

template <class T>
void UniquePtr<T>::reset(T *ptr) noexcept {
	delete pointer;
	pointer = ptr;
}

template <class T>
UniquePtr<T>::operator bool() const noexcept {
	return pointer != nullptr;
}

template <class T>
T *UniquePtr<T>::operator->() const noexcept {
	return pointer;
}

template <class T>
T &UniquePtr<T>::operator*() const {
	return *pointer;
}

template <class T>
T *UniquePtr<T>::get() const noexcept {
	return pointer;
}

template <class T>
UniquePtr<T>::~UniquePtr() {
	delete pointer;
}

template <class T>
UniquePtr<T>::UniquePtr(T *ptr) noexcept : pointer{ptr} {}
}  // namespace my_smart_ptr