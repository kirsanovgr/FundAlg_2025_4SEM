#ifndef FUNDAMENTAL_ALGORITHMS_2_LIST_H
#define FUNDAMENTAL_ALGORITHMS_2_LIST_H

#include <limits>

#include "../../container.h"

namespace my_container {
    template<typename T>
    class List : public Container<T> {
    private:
        struct Node {
            T data;
            Node *prev;
            Node *next;
            explicit Node(const T& val = T(), Node* p = nullptr, Node* n = nullptr) : data(val), prev(p), next(n) {}
        };

        Node *head;
        Node *tail;

        size_t list_size {};

    public:
        List() : head(nullptr), tail(nullptr), list_size(0) {}
        List(const std::initializer_list<T> init) : List() {
            for (const auto &item : init) {
                push_back(item);
            }
        }
        List(const List& other) : List() {
            for (const auto &item : other) {
                push_back(item);
            }
        }
        List(List&& other) noexcept : head(other.head), tail(other.tail), list_size(other.list_size) {
            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }

        ~List() override {
            clear();
        }

        virtual List& operator=(const List& other) {
            if (this != &other) {
                List temp(other);
                swap(temp);
            }
            return *this;
        }

        virtual List& operator=(List&& other) noexcept {
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
            const List* other_list = dynamic_cast<const List*>(&other);
            if (!other_list) {
                throw std::invalid_argument("List");
            }
            *this = *other_list;
            return *this;
        }

        T& front() {
            if (empty()) throw std::out_of_range("List");
            return head->data;
        }

        const T& front() const {
            if (empty()) throw std::out_of_range("List");
            return head->data;
        }

        T& back() {
            if (empty()) throw std::out_of_range("List");
            return tail->data;
        }

        const T& back() const {
            if (empty()) throw std::out_of_range("List");
            return tail->data;
        }

        class iterator {
            friend class List;
            Node* ptr;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            explicit iterator(Node* p = nullptr) : ptr(p) {}
            iterator& operator++() { ptr = ptr->next; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
            iterator& operator--() { ptr = ptr->prev; return *this; }
            iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }
            bool operator==(const iterator& other) const { return ptr == other.ptr; }
            bool operator!=(const iterator& other) const { return !(*this == other); }
            T& operator*() const { return ptr->data; }
            T* operator->() const { return &ptr->data; }
        };

        iterator begin() { return iterator(head); }
        iterator end() { return iterator(nullptr); }

        iterator begin() const { return iterator(head); }
        iterator end() const { return iterator(nullptr); }

        class const_iterator {
            const Node* ptr;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            explicit const_iterator(const Node* p = nullptr) : ptr(p) {}
            const_iterator& operator++() { ptr = ptr->next; return *this; }
            const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
            const_iterator& operator--() { ptr = ptr->prev; return *this; }
            const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }
            bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
            bool operator!=(const const_iterator& other) const { return !(*this == other); }
            const T& operator*() const { return ptr->data; }
            const T* operator->() const { return &ptr->data; }
        };

        const_iterator cbegin() const { return const_iterator(head); }
        const_iterator cend() const { return const_iterator(nullptr); }

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        reverse_iterator rbegin() noexcept {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(cend());
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(begin());
        }
        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(cbegin());
        }

        bool empty() const override { return list_size == 0; }
        size_t size() const override { return list_size; }
        size_t max_size() const override { return std::numeric_limits<size_t>::max(); }

        void clear() {
            while (!empty()) {
                pop_front();
            }
        }

        iterator insert(iterator pos, const T& value) {
            Node* current = pos.ptr;
            Node* newNode = new Node(value, current ? current->prev : tail, current);
            if (newNode->prev) {
                newNode->prev->next = newNode;
            } else {
                head = newNode;
            }
            if (newNode->next) {
                newNode->next->prev = newNode;
            } else {
                tail = newNode;
            }
            list_size++;
            return iterator(newNode);
        }

        iterator erase(iterator pos) {
            if (pos == end()) return pos;
            Node* current = pos.ptr;
            Node* nextNode = current->next;
            if (current->prev) {
                current->prev->next = nextNode;
            } else {
                head = nextNode;
            }
            if (nextNode) {
                nextNode->prev = current->prev;
            } else {
                tail = current->prev;
            }
            delete current;
            list_size--;
            return iterator(nextNode);
        }

        void push_back(const T& value) {
            insert(end(), value);
        }

        void pop_back() {
            erase(iterator(tail));
        }

        void push_front(const T& value) {
            insert(begin(), value);
        }

        void pop_front() {
            erase(begin());
        }

        void resize(size_t count, const T& value = T()) {
            while (size() > count) pop_back();
            while (size() < count) push_back(value);
        }

        void swap(List& other) noexcept {
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            std::swap(list_size, other.list_size);
        }

        bool operator==(const Container<T>& other) const override {
            const List* otherList = dynamic_cast<const List*>(&other);
            if (!otherList || size() != otherList->size()) return false;
            return std::equal(cbegin(), cend(), otherList->cbegin());
        }

        bool operator!=(const Container<T>& other) const override {
            return !(*this == other);
        }

        virtual bool operator<(const List& other) const {
            return std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
        }

        virtual std::strong_ordering operator<=>(const List& other) const {
            return std::lexicographical_compare_three_way(cbegin(), cend(), other.cbegin(), other.cend());
        }
    };
}

#endif //FUNDAMENTAL_ALGORITHMS_2_LIST_H
