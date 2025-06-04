//
// Created by Олеся on 09.05.2025.
//

#ifndef DEQUE_LIST_HPP
#define DEQUE_LIST_HPP

#include <iostream>
#include <limits>
#include <iterator>
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <compare>

template <typename T>
class Container {
public:
    Container() = default;
    Container(const Container& other) = default;
    virtual Container& operator=(const Container& other) = 0;
    virtual ~Container() = 0;

    virtual bool operator==(const Container& other) const = 0;
    virtual bool operator!=(const Container& other) const = 0;

    virtual std::size_t size() const = 0;
    virtual std::size_t max_size() const = 0;
    virtual bool empty() const = 0;
};

template <typename T>
Container<T>:: ~Container() = default;

template<typename T>
class List : public Container<T> {
    struct Node {
        T data;
        Node *prev;
        Node *next;

        Node(const T &val, Node *p = nullptr, Node *n = nullptr)
                : data(val), prev(p), next(n) {}
        Node(T &&val, Node *p = nullptr, Node *n = nullptr)
                : data(std::move(val)), prev(p), next(n) {}
    };

    Node *head;
    Node *tail;
    std::size_t list_size;

    Node* find_node(const T *pos) const {
        for (Node *curr = head; curr; curr = curr->next) {
            if (&curr->data == pos) return curr;
        }
        return nullptr;
    }

public:
    List() : head(nullptr), tail(nullptr), list_size(0) {}

    List(const List &other) : List() {
        for (const T &item : other) push_back(item);
    }

    List(List &&other) noexcept
            : head(other.head), tail(other.tail), list_size(other.list_size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }

    List(std::initializer_list<T> init) : List() {
        for (const T &item : init) push_back(item);
    }

    ~List() override { clear(); }

    Container<T>& operator=(const Container<T>& other) override {
        if (this != &other) {
            auto* vec = dynamic_cast<const List*>(&other);
            if (!vec) {
                throw std::invalid_argument("Assigned Container must be of type List");
            }
            *this = *vec;
        }
        return *this;
    }

    List &operator=(const List &other) {
        if (this != &other) {
            List tmp(other);
            swap(tmp);
        }
        return *this;
    }

    List &operator=(List &&other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;
            other.head = other.tail = nullptr;
            other.list_size = 0;
        }
        return *this;
    }

    bool empty() const noexcept override { return list_size == 0; }
    std::size_t size() const noexcept override { return list_size; }
    std::size_t max_size() const noexcept override { return std::numeric_limits<std::size_t>::max(); }

    void push_back(const T &value) {
        Node *n = new Node(value, tail);
        if (tail) tail->next = n;
        else head = n;
        tail = n;
        ++list_size;
    }

    void push_back(T &&value) {
        Node *n = new Node(std::move(value), tail);
        if (tail) tail->next = n;
        else head = n;
        tail = n;
        ++list_size;
    }

    void push_front(const T &value) {
        Node *n = new Node(value, nullptr, head);
        if (head) head->prev = n;
        else tail = n;
        head = n;
        ++list_size;
    }

    void push_front(T &&value) {
        Node *n = new Node(std::move(value), nullptr, head);
        if (head) head->prev = n;
        else tail = n;
        head = n;
        ++list_size;
    }

    void pop_back() {
        if (!tail) return;
        Node *to_delete = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete to_delete;
        --list_size;
    }

    void pop_front() {
        if (!head) return;
        Node *to_delete = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete to_delete;
        --list_size;
    }

    void clear() {
        while (!empty()) pop_front();
    }

    T &front() {
        if (!head) throw std::out_of_range("List is empty");
        return head->data;
    }

    const T &front() const {
        if (!head) throw std::out_of_range("List is empty");
        return head->data;
    }

    T &back() {
        if (!tail) throw std::out_of_range("List is empty");
        return tail->data;
    }

    const T &back() const {
        if (!tail) throw std::out_of_range("List is empty");
        return tail->data;
    }

    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node *ptr) : node(ptr) {}

        reference operator*() const { return node->data; }
        pointer operator->() const { return &node->data; }

        iterator &operator++() {
            node = node->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator &operator--() {
            node = node->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        friend bool operator==(const iterator &a, const iterator &b) {
            return a.node == b.node;
        }

        friend bool operator!=(const iterator &a, const iterator &b) {
            return !(a == b);
        }

    private:
        Node *node;
    };
    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const Node* ptr) : node(ptr) {}

        reference operator*() const { return node->data; }
        pointer operator->() const { return &node->data; }

        const_iterator& operator++() {
            node = node->next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const_iterator& operator--() {
            node = node->prev;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        friend bool operator==(const const_iterator& a, const const_iterator& b) {
            return a.node == b.node;
        }

        friend bool operator!=(const const_iterator& a, const const_iterator& b) {
            return !(a == b);
        }

    private:
        const Node* node;
    };

    iterator begin() noexcept { return iterator(head); }
    iterator end() noexcept { return iterator(nullptr); }

    const_iterator begin() const noexcept { return const_iterator(head); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }

    const_iterator cbegin() const noexcept { return const_iterator(head); }
    const_iterator cend() const noexcept { return const_iterator(nullptr); }


    T *insert(const T *pos, const T &value) {
        if (!pos || pos == &head->data) {
            push_front(value);
            return &head->data;
        }
        if (pos == &tail->data + 1) {
            push_back(value);
            return &tail->data;
        }

        Node *curr = find_node(pos);
        if (!curr) return nullptr;

        Node *n = new Node(value, curr->prev, curr);
        curr->prev->next = n;
        curr->prev = n;
        ++list_size;
        return &n->data;
    }

    T *erase(const T *pos) {
        Node *curr = find_node(pos);
        if (!curr) return nullptr;

        if (curr == head) {
            pop_front();
            return head ? &head->data : nullptr;
        }
        if (curr == tail) {
            pop_back();
            return nullptr;
        }

        Node *next = curr->next;
        curr->prev->next = curr->next;
        if (curr->next) curr->next->prev = curr->prev;
        delete curr;
        --list_size;
        return next ? &next->data : nullptr;
    }

    void resize(std::size_t count, const T &value = T()) {
        while (list_size > count) pop_back();
        while (list_size < count) push_back(value);
    }

    void swap(List &other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(list_size, other.list_size);
    }
    bool operator==(const List<T>& other) const {
        if (list_size != other.list_size) return false;

        Node* a = head;
        Node* b = other.head;

        while (a && b) {
            if (!(a->data == b->data)) return false;
            a = a->next;
            b = b->next;
        }
        return true;
    }

    bool operator==(const Container<T>& other) const override {
        const List<T>* otherList = dynamic_cast<const List<T>*>(&other);
        if (!otherList) return false;
        return *this == *otherList;
    }


    std::strong_ordering operator<=>(const List &other) const {
        Node *a = head, *b = other.head;
        while (a && b) {
            if (auto cmp = a->data <=> b->data; cmp != 0) return cmp;
            a = a->next;
            b = b->next;
        }
        return list_size <=> other.list_size;
    }

    bool operator!=(const Container<T>& other) const override {
        return !(*this == other);
    }
    bool operator<(const List &other) const { return (*this <=> other) < 0; }
    bool operator<=(const List &other) const { return (*this <=> other) <= 0; }
    bool operator>(const List &other) const { return (*this <=> other) > 0; }
    bool operator>=(const List &other) const { return (*this <=> other) >= 0; }
};


#endif //DEQUE_LIST_HPP
