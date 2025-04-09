#pragma once
#include <iostream>

namespace my_smart_ptr {

template<typename T>
class UniquePtr {
private:
    T* raw_ptr;
public:
    UniquePtr();
    UniquePtr(T* ptr);
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr(UniquePtr& other) = delete;
    ~UniquePtr();
    

    UniquePtr<T>& operator=(const UniquePtr& other) = delete;
    UniquePtr<T>& operator=(UniquePtr&& other) noexcept;

    T* get() const noexcept;
    T& operator*() const;
    T* operator->() const noexcept;
    explicit operator bool() const noexcept;

    void reset(T* ptr = nullptr) noexcept;
    T* release() noexcept;
    void swap(UniquePtr& other) noexcept;



};

template<typename T>
UniquePtr<T>::~UniquePtr() {
    delete raw_ptr;
}

template<typename T>
UniquePtr<T>::UniquePtr() : raw_ptr() {}


template<typename T>
UniquePtr<T>::UniquePtr(T* ptr) : raw_ptr(ptr) {}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept : raw_ptr(other.raw_ptr) {
    other.raw_ptr = nullptr;
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
    if (raw_ptr != other.raw_ptr) {
        raw_ptr = other.raw_ptr;
        other.raw_ptr = nullptr;
    }
    return *this;
}

template<typename T>
T* UniquePtr<T>::get() const noexcept {
    return raw_ptr;
}

template<typename T>
T& UniquePtr<T>::operator*() const {
    return *raw_ptr;
}

template<typename T>
T* UniquePtr<T>::operator->() const noexcept {
    return raw_ptr;
}

template<typename T>
UniquePtr<T>::operator bool() const noexcept {
    return raw_ptr != nullptr;

}

template<typename T>
void UniquePtr<T>::reset(T* ptr) noexcept {
    if (raw_ptr != ptr) {
        delete raw_ptr;
        raw_ptr = ptr;
    }

}

template<typename T>
T* UniquePtr<T>::release() noexcept {
    T* tmp = raw_ptr;
    raw_ptr = nullptr;
    return tmp;

}

template<typename T>
void UniquePtr<T>::swap(UniquePtr& other) noexcept {
    std::swap(raw_ptr, other.raw_ptr);

}

//
//
// специализация для массивов
template<typename T>
class UniquePtr<T[]> {
private:
    T* raw_ptr;
public:
    UniquePtr();
    UniquePtr(T* ptr);
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr(UniquePtr& other) = delete;
    ~UniquePtr();
    

    UniquePtr<T[]>& operator=(const UniquePtr& other) = delete;
    UniquePtr<T[]>& operator=(UniquePtr&& other) noexcept;

    T* get() const noexcept;
    // для массивов не надо(даже нельзя), тк есть operator[]
    T& operator*() const = delete;
    T* operator->() const noexcept = delete;
    T& operator[](size_t i) const;
    explicit operator bool() const noexcept;

    void reset(T* ptr = nullptr) noexcept;
    T* release() noexcept;
    void swap(UniquePtr& other) noexcept;



};

template<typename T>
UniquePtr<T[]>::~UniquePtr() {
    delete[] raw_ptr;
}

template<typename T>
UniquePtr<T[]>::UniquePtr() : raw_ptr() {}


template<typename T>
UniquePtr<T[]>::UniquePtr(T* ptr) : raw_ptr(ptr) {}

template<typename T>
UniquePtr<T[]>::UniquePtr(UniquePtr&& other) noexcept : raw_ptr(other.raw_ptr) {
    other.raw_ptr = nullptr;
}

template<typename T>
UniquePtr<T[]>& UniquePtr<T[]>::operator=(UniquePtr&& other) noexcept {
    if (raw_ptr != other.raw_ptr) {
        raw_ptr = other.raw_ptr;
        other.raw_ptr = nullptr;
    }
}

template<typename T>
T* UniquePtr<T[]>::get() const noexcept {
    return raw_ptr;
}

template<typename T>
UniquePtr<T[]>::operator bool() const noexcept {
    return raw_ptr != nullptr;

}

template<typename T>
void UniquePtr<T[]>::reset(T* ptr) noexcept {
    if (raw_ptr != ptr) {
        delete[] raw_ptr;
        raw_ptr = ptr;
    }

}

template<typename T>
T* UniquePtr<T[]>::release() noexcept {
    T* tmp = raw_ptr;
    raw_ptr = nullptr;
    return tmp;

}

template<typename T>
void UniquePtr<T[]>::swap(UniquePtr& other) noexcept {
    std::swap(raw_ptr, other.raw_ptr);

}

template<typename T>
T& UniquePtr<T[]>::operator[](size_t i) const {
    return raw_ptr[i];

}






}