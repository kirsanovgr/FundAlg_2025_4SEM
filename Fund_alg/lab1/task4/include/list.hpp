
#pragma once

#include <initializer_list>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include "Container.hpp"


namespace my_container {

    template<typename T>
    class List : public Container<T> {
    private:
        struct Node {
            T data;
            Node* prev;
            Node* next;
            explicit Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
        };

        Node* head;
        Node* tail;
        size_t list_size;

    public:
        class iterator {
        public:
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::bidirectional_iterator_tag;

            explicit iterator(Node* node = nullptr, const List* lst = nullptr)
                    : current(node), list(lst) {}



            Node* getCurrent() const {return current; }

            reference operator*() const { return current->data; }

            iterator& operator++() {
                if (current) {
                    current = current->next;
                }
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            iterator& operator--() {
                if (current) {
                    current = current->prev;
                } else {
                    current = list->tail;
                }
                return *this;
            }

            iterator operator--(int) {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const iterator& other) const { return current == other.current; }
            bool operator!=(const iterator& other) const { return current != other.current; }

            friend class List;

        private:
            Node* current;
            const List* list;
        };

        class const_iterator {
        public:
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;
            using iterator_category = std::bidirectional_iterator_tag;

            explicit const_iterator(Node* node = nullptr, const List* lst = nullptr)
                    : current(node), list(lst) {}

            reference operator*() const { return current->data; }

            const_iterator& operator++() {
                if (current) {
                    current = current->next;
                }
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            const_iterator& operator--() {
                if (current) {
                    current = current->prev;
                } else {
                    current = list->tail;
                }
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const const_iterator& other) const { return current == other.current; }
            bool operator!=(const const_iterator& other) const { return current != other.current; }

            friend class List;

        private:
            Node* current;
            const List* list;
        };

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        List() : head(nullptr), tail(nullptr), list_size(0) {}

        List(const List& other) : head(nullptr), tail(nullptr), list_size(0) {
            for (const auto& elem : other) {
                push_back(elem);
            }
        }

        List(List&& other) noexcept : head(other.head), tail(other.tail), list_size(other.list_size) {
            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }

        List(std::initializer_list<T> init) : List() {
            for (const auto& elem : init) {
                push_back(elem);
            }
        }

        ~List() {
            clear();
        }

        List& operator=(const List& other) {
            if (this != &other) {
                List temp(other);
                swap(temp);
            }
            return *this;
        }

        List& operator=(List&& other) noexcept {
            if (this != &other) {
                clear();
                head = other.head;
                tail = other.tail;
                list_size = other.list_size;
                other.head = nullptr;
                other.tail = nullptr;
                other.list_size = 0;
            }
            return *this;
        }

        Container<T>& operator=(const Container<T>& other) override {
            if (*this == other) {
                return *this;
            }

            const List* otherList = dynamic_cast<const List*>(&other);
            if (!otherList) {
                throw std::bad_cast();
            }
            *this = *otherList;
            return *this;
        }

        bool operator==(const Container<T>& other) const override {
            const List* otherList = dynamic_cast<const List*>(&other);
            if (!otherList) return false;
            return list_size == otherList->list_size && std::equal(begin(), end(), otherList->begin());
        }

        bool operator!=(const Container<T>& other) const override {
            return !(*this == other);
        }

        bool operator==(const List<T>& other) const {
            return (*this <=> other) == std::strong_ordering::equal;
        }

        auto operator<=>(const List<T>& other) const {
            return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
        }

        T& front() {
            if (empty()) throw std::out_of_range("List is empty");
            return head->data;
        }

        const T& front() const {
            if (empty()) throw std::out_of_range("List is empty");
            return head->data;
        }

        T& back() {
            if (empty()) throw std::out_of_range("List is empty");
            return tail->data;
        }

        const T& back() const {
            if (empty()) throw std::out_of_range("List is empty");
            return tail->data;
        }

        iterator begin() { return iterator(head, this); }
        const_iterator begin() const { return const_iterator(head, this); }
        const_iterator cbegin() const { return begin(); }

        iterator end() { return iterator(nullptr, this); }
        const_iterator end() const { return const_iterator(nullptr, this); }
        const_iterator cend() const { return end(); }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const { return rbegin(); }

        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const { return rend(); }

        size_t size() const override {
            return list_size;
        }

        size_t max_size() const override {
            return std::numeric_limits<size_t>::max();
        }

        bool empty() const override {
            return list_size == 0;
        }

        void clear() {
            Node* current = head;
            while (current) {
                Node* next = current->next;
                delete current;
                current = next;
            }
            head = tail = nullptr;
            list_size = 0;
        }

        iterator insert(iterator pos, const T& value) {
            if (pos == end()) {
                push_back(value);
                return iterator(tail);
            }
            Node* current = pos.current;
            Node* newNode = new Node(value);
            newNode->prev = current->prev;
            newNode->next = current;
            if (current->prev) {
                current->prev->next = newNode;
            } else {
                head = newNode;
            }
            current->prev = newNode;
            ++list_size;
            return iterator(newNode);
        }

        iterator erase(iterator pos) {
            if (pos == end()) return pos;
            Node* current = pos.current;
            Node* nextNode = current->next;
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev;
            }
            delete current;
            --list_size;
            return iterator(nextNode);
        }

        void push_back(const T& value) {
            Node* newNode = new Node(value);
            if (!tail) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            ++list_size;
        }

        void push_front(const T& value) {
            Node* newNode = new Node(value);
            if (!head) {
                head = tail = newNode;
            } else {
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }
            ++list_size;
        }

        void pop_back() {
            if (empty()) return;
            Node* toDelete = tail;
            tail = tail->prev;
            if (tail) {
                tail->next = nullptr;
            } else {
                head = nullptr;
            }
            delete toDelete;
            --list_size;
        }

        void pop_front() {
            if (empty()) return;
            Node* toDelete = head;
            head = head->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
            delete toDelete;
            --list_size;
        }

        void resize(size_t n, const T& value = T()) {
            while (list_size > n) {
                pop_back();
            }
            while (list_size < n) {
                push_back(value);
            }
        }

        void swap(List& other) noexcept {
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            std::swap(list_size, other.list_size);
        }

        Node* getHead() const { return head; }
        Node* getTail() const { return tail; }
    };
}