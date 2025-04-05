#ifndef FUNDAMENTAL_ALGORITHMS_2_UNIQUE_PTR_H
#define FUNDAMENTAL_ALGORITHMS_2_UNIQUE_PTR_H

#include <iostream>

namespace my_smart_ptr {

    template <typename T>
    class UniquePtr {
    private:
        T* _ptr = nullptr;

    public:
        UniquePtr() noexcept = default;
        explicit UniquePtr(T* ptr) noexcept : _ptr(ptr) {}
        UniquePtr(UniquePtr&& other) noexcept : _ptr(other._ptr) {
            other._ptr = nullptr;
        }

        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr& operator=(UniquePtr&& other) noexcept {
            if (this != &other) {
                reset();
                _ptr = other._ptr;
                other._ptr = nullptr;
            }
            return *this;
        }

        ~UniquePtr() {
            delete _ptr;
        }

        T* get() const noexcept { return _ptr; }
        T& operator*() const { return *_ptr; }
        T* operator->() const noexcept { return _ptr; }
        explicit operator bool() const noexcept { return _ptr != nullptr; }

        void reset(T* ptr = nullptr) noexcept {
            delete _ptr;
            _ptr = ptr;
        }

        T* release() noexcept {
            T* tmp = _ptr;
            _ptr = nullptr;
            return tmp;
        }

        void swap(UniquePtr& other) noexcept {
            std::swap(_ptr, other._ptr);
        }
    };

    template <typename T>
    class UniquePtr<T[]> {
    private:
        T* _ptr = nullptr;

    public:
        UniquePtr() noexcept = default;
        explicit UniquePtr(T* ptr) noexcept : _ptr(ptr) {}
        UniquePtr(UniquePtr&& other) noexcept : _ptr(other._ptr) {
            other._ptr = nullptr;
        }

        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr& operator=(UniquePtr&& other) noexcept {
            if (this != &other) {
                reset();
                _ptr = other._ptr;
                other._ptr = nullptr;
            }
            return *this;
        }

        ~UniquePtr() {
            delete[] _ptr;
        }

        T* get() const noexcept { return _ptr; }
        T& operator[](size_t index) const { return _ptr[index]; }
        explicit operator bool() const noexcept { return _ptr != nullptr; }

        void reset(T* ptr = nullptr) noexcept {
            delete[] _ptr;
            _ptr = ptr;
        }

        T* release() noexcept {
            T* tmp = _ptr;
            _ptr = nullptr;
            return tmp;
        }

        void swap(UniquePtr& other) noexcept {
            std::swap(_ptr, other._ptr);
        }
    };

}


#endif //FUNDAMENTAL_ALGORITHMS_2_UNIQUE_PTR_H
