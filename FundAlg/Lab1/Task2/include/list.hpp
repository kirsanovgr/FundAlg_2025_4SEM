#ifndef LIST_H
#define LIST_H

#include "container.hpp"
#include <limits>

namespace my_container {
    template<typename T>
    class List : public Container<T> {
    private:
        struct Node {
            T data;
            Node *prev;
            Node *next;

            explicit Node(const T &val = T(), Node *p = nullptr, Node *n = nullptr) : data(val), prev(p), next(n) {
            }
        };

        Node *head;
        Node *tail;

        size_t list_size = 0;

    public:
        List() : head(nullptr), tail(nullptr), list_size(0) {
        }

        List(const List &other) : List() {
            for (const auto &item: other) {
                push_back(item);
            }
        }

        List(List &&other) noexcept : head(other.head), tail(other.tail), list_size(other.list_size) {
            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }

        List(const std::initializer_list<T> init) : List() {
            for (const auto &item: init) {
                push_back(item);
            }
        }

        ~List() override {
            clear();
        }

        virtual List &operator=(const List &other) {
            if (this != &other) {
                List temp(other);
                swap(temp);
            }
            return *this;
        }

        virtual List &operator=(List &&other) noexcept {
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

        Container<T> &operator=(const Container<T> &other) override {
            const List *other_list = dynamic_cast<const List *>(&other);
            if (!other_list) {
                throw std::invalid_argument("Invalid type");
            }
            *this = *other_list;
            return *this;
        }

        T &front() {
            if (empty()) throw std::out_of_range("List is empty");
            return head->data;
        }

        const T &front() const {
            if (empty()) throw std::out_of_range("List is empty");
            return head->data;
        }

        T &back() {
            if (empty()) throw std::out_of_range("List is empty");
            return tail->data;
        }

        const T &back() const {
            if (empty()) throw std::out_of_range("List is empty");
            return tail->data;
        }

        class iterator {
            friend class List;
            Node *ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            explicit iterator(Node *p = nullptr) : ptr(p) {
            }

            reference operator*() const { return ptr->data; }
            pointer operator->() const { return &ptr->data; }

            iterator &operator++() {
                ptr = ptr->next;
                return *this;
            }

            iterator operator++(int) {
                iterator temp = *this;
                ++(*this);
                return temp;
            }

            iterator &operator--() {
                ptr = ptr->prev;
                return *this;
            }

            iterator operator--(int) {
                iterator temp = *this;
                --(*this);
                return temp;
            }

            bool operator==(const iterator &other) const { return ptr == other.ptr; }
            bool operator!=(const iterator &other) const { return !(*this == other); }
        };

        class const_iterator {
            friend class List;
            const Node *ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = const T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T *;
            using reference = const T &;

            explicit const_iterator(const Node *p = nullptr) : ptr(p) {
            }

            reference operator*() const { return ptr->data; }
            pointer operator->() const { return &ptr->data; }

            const_iterator &operator++() {
                ptr = ptr->next;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator temp = *this;
                ++(*this);
                return temp;
            }

            const_iterator &operator--() {
                ptr = ptr->prev;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator temp = *this;
                --(*this);
                return temp;
            }

            bool operator==(const const_iterator &other) const { return ptr == other.ptr; }
            bool operator!=(const const_iterator &other) const { return !(*this == other); }
        };

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        iterator begin() noexcept { return iterator(head); }
        const_iterator begin() const noexcept { return const_iterator(head); }
        const_iterator cbegin() const noexcept { return const_iterator(head); }

        iterator end() noexcept { return iterator(nullptr); }
        const_iterator end() const noexcept { return const_iterator(nullptr); }
        const_iterator cend() const noexcept { return const_iterator(nullptr); }

        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

        bool empty() const noexcept override { return list_size == 0; }
        size_t size() const noexcept override { return list_size; }
        size_t max_size() const noexcept override { return std::numeric_limits<size_t>::max(); }

        void clear() {
            while (!empty()) {
                pop_back();
            }
        }

        iterator insert(const_iterator pos, const T &value) {
            Node *next_node = const_cast<Node *>(pos.ptr);
            Node *prev_node = nullptr;

            if (next_node == nullptr) {
                prev_node = tail;
            } else {
                prev_node = next_node->prev;
            }

            Node *new_node = new Node(value, prev_node, next_node);

            if (prev_node != nullptr) {
                prev_node->next = new_node;
            } else {
                head = new_node;
            }

            if (next_node != nullptr) {
                next_node->prev = new_node;
            } else {
                tail = new_node;
            }

            list_size++;
            return iterator(new_node);
        }

        iterator erase(const_iterator pos) {
            Node *current = const_cast<Node *>(pos.ptr);
            if (current == nullptr) {
                return end();
            }

            Node *prev_node = current->prev;
            Node *next_node = current->next;

            if (prev_node != nullptr) {
                prev_node->next = next_node;
            } else {
                head = next_node;
            }

            if (next_node != nullptr) {
                next_node->prev = prev_node;
            } else {
                tail = prev_node;
            }

            delete current;
            list_size--;
            return iterator(next_node);
        }

        void push_back(const T &value) {
            insert(cend(), value);
        }

        void pop_back() {
            erase(const_iterator(tail));
        }

        void push_front(const T &value) {
            insert(cbegin(), value);
        }

        void pop_front() {
            erase(cbegin());
        }

        void resize(size_t new_size, const T &value = T()) {
            while (list_size > new_size) {
                pop_back();
            }
            while (list_size < new_size) {
                push_back(value);
            }
        }

        void swap(List &other) noexcept {
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            std::swap(list_size, other.list_size);
        }

        bool operator==(const Container<T> &other) const override {
            const List *other_list = dynamic_cast<const List *>(&other);
            if (!other_list || size() != other_list->size()) return false;

            return std::equal(cbegin(), cend(), other_list->cbegin());
        }

        bool operator!=(const Container<T> &other) const override {
            return !(*this == other);
        }

        std::strong_ordering operator<=>(const List &other) const {
            return std::lexicographical_compare_three_way(cbegin(), cend(),
                                                          other.cbegin(), other.cend());
        }
    };
}

#endif //LIST_H
