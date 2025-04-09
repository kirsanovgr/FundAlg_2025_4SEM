#pragma once
#include <initializer_list>
#include <iterator>
#include "container.h"

namespace my_container {

template<typename T>
class List : public Container<T> {
private:
    typedef struct Node_ {
        T data;
        struct Node_* left;
        struct Node_* right;

    } Node;

    Node* start_;
    Node* end_;
    size_t size_ ;

public:

    class iterator {
	   public:
        // требования стандартного итератора
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        Node* node;
		explicit iterator(Node *node);
		T& operator*();
		T* operator->();
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
	};

	class const_iterator {
	   public:
        // требования стандартного итератора
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

		const Node* node;
		explicit const_iterator(const Node* node);
		const T& operator*() const;
		const T* operator->() const;
        const_iterator& operator++();
		const_iterator operator++(int);
		const_iterator& operator--();
		const_iterator operator--(int);
		bool operator==(const const_iterator& other) const;
		bool operator!=(const const_iterator& other) const;
	};

    List();
    List(const List<T>& other);
    List(List<T>&& other) noexcept;
    List(std::initializer_list<T> initl);
    ~List();

    List<T>& operator=(const List<T>& other);

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    size_t size() const override;
    bool empty() const override;

    void clear();
    void push_back(const T& value);
    void pop_back();
    void push_front(const T& value);
    void pop_front();
    void resize(size_t count);
    void resize(size_t count, const T& value);
    void swap(List<T>& other);
    List<T>::iterator insert(List<T>::iterator pos, const T& value);
    List<T>::iterator erase(List<T>::iterator pos);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    std::reverse_iterator<iterator> rbegin();
    std::reverse_iterator<const_iterator> rbegin() const;
    std::reverse_iterator<const_iterator> crbegin() const;
    std::reverse_iterator<iterator> rend();
    std::reverse_iterator<const_iterator> rend() const;
    std::reverse_iterator<const_iterator> crend() const;

    std::strong_ordering operator<=>(const List<T> &rhs) const;
    bool operator==(const List<T>& rhs) const;





};

template<typename T>
List<T>::List() {
    start_ = new Node(T(), nullptr, nullptr);
    end_ = new Node(T(), start_, nullptr);
    start_->right = end_;
    size_ = 0;

}

template<typename T>
List<T>::List(const List<T>& other) : List() {
    List<T>::const_iterator iter = other.begin();
    while(iter != other.end()) {
        push_back(*iter);
        iter++;
    }

}

template <typename T>
List<T>::List(std::initializer_list<T> initl) : List() {
    for (const auto& element : initl) {
        push_back(element);
    }

}

template<typename T>
List<T>::~List() {
    clear();
    delete start_;
    delete end_;
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (*this != other) {
        clear();
        List<T>::const_iterator iter = other.begin();
        while(iter != other.end()) {
            push_back(*iter);
            iter++;
        }
        size_ = other.size_;
    }
    return *this;

}

template<typename T>
List<T>::List(List<T>&& other) noexcept :
    start_(other.start_), end_(other.end_), size_(other.size_) {
        other.start_ = nullptr;
        other.end_ = nullptr;
        other.size_ = 0;
    }

template<typename T>
List<T>::iterator::iterator(List<T>::Node* current_node) : node{current_node} {};

template<typename T>
List<T>::const_iterator::const_iterator(const List<T>::Node* current_node) : node{current_node} {};

template<typename T>
List<T>::iterator List<T>::begin() {
    return List<T>::iterator(start_->right);
}

template<typename T>
List<T>::const_iterator List<T>::begin() const {
    return List<T>::const_iterator(start_->right);
}

template<typename T>
List<T>::iterator List<T>::end() {
    return List<T>::iterator(end_);
}

template<typename T>
List<T>::const_iterator List<T>::end() const {
    return List<T>::const_iterator(end_);
}

template<typename T>
List<T>::const_iterator List<T>::cbegin() const noexcept {
    return List<T>::const_iterator(start_->right);
}

template<typename T>
List<T>::const_iterator List<T>::cend() const noexcept {
    return List<T>::const_iterator(end_);
}

template<typename T>
// явно указание, что это тип
std::reverse_iterator<typename List<T>::iterator> List<T>::rbegin() {
    return std::reverse_iterator(end());

}

template<typename T>
std::reverse_iterator<typename List<T>::const_iterator> List<T>::rbegin() const {
    return std::reverse_iterator(cend());

}

template<typename T>
std::reverse_iterator<typename List<T>::const_iterator> List<T>::crbegin() const {
    return std::reverse_iterator(cend());

}

template<typename T>
std::reverse_iterator<typename List<T>::iterator> List<T>::rend() {
    return std::reverse_iterator(begin());

}

template<typename T>
std::reverse_iterator<typename List<T>::const_iterator> List<T>::rend() const {
    return std::reverse_iterator(cbegin());

}

template<typename T>
std::reverse_iterator<typename List<T>::const_iterator> List<T>::crend() const {
    return std::reverse_iterator(cend());

}

template<typename T>
bool List<T>::empty() const{
    return size_ == 0;
}

template<typename T>
size_t List<T>::size() const {
    return size_;

}

template<typename T>
T& List<T>::front() {
    return start_->right->data;
}

template<typename T>
const T& List<T>::front() const {
    return start_->right->data;
}

template<typename T>
T& List<T>::back() {
    return end_->left->data;
}

template<typename T>
const T& List<T>::back() const {
    return end_->left->data;
}

template<typename T>
T& List<T>::iterator::operator*() {
    return node->data;
}

template<typename T>
T* List<T>::iterator::operator->() {
    return &(node->data);
}

template<typename T>
const T& List<T>::const_iterator::operator*() const {
    return node->data;
}

template<typename T>
const T* List<T>::const_iterator::operator->() const {
    return &(node->data);
}

template<typename T>
List<T>::iterator& List<T>::iterator::operator++() {
    node = node->right;
    return (*this);
}

template<typename T>
List<T>::iterator List<T>::iterator::operator++(int) {
    List<T>::iterator tmp = *this;
    ++(*this);
    return tmp;
}

template<typename T>
List<T>::iterator& List<T>::iterator::operator--() {
    node = node->left;
    return (*this);
}

template<typename T>
List<T>::iterator List<T>::iterator::operator--(int) {
    List<T>::iterator tmp = *this;
    --(*this);
    return tmp;
}

template<typename T>
List<T>::const_iterator& List<T>::const_iterator::operator++() {
    node = node->right;
    return (*this);
}

template<typename T>
List<T>::const_iterator List<T>::const_iterator::operator++(int) {
    List<T>::const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

template<typename T>
List<T>::const_iterator& List<T>::const_iterator::operator--() {
    node = node->left;
    return (*this);
}

template<typename T>
List<T>::const_iterator List<T>::const_iterator::operator--(int) {
    List<T>::iterator tmp = *this;
    --(*this);
    return tmp;
}

template<typename T>
bool List<T>::iterator::operator==(const List<T>::iterator& other) const {
    return node == other.node;
}

template<typename T>
bool List<T>::iterator::operator!=(const List<T>::iterator& other) const {
    return (node == other.node) == false;
}

template<typename T>
bool List<T>::const_iterator::operator==(const List<T>::const_iterator& other) const {
    return node == other.node;
}

template<typename T>
bool List<T>::const_iterator::operator!=(const List<T>::const_iterator& other) const {
    return (node == other.node) == false;
}

template<typename T>
void List<T>::push_back(const T& value) {
    Node* new_node = new Node(value, end_->left, end_);
    end_->left->right = new_node;
    end_->left = new_node;
    size_++;
}

template<typename T>
void List<T>::push_front(const T& value) {
    Node* new_node = new Node(value, start_, start_->right);
    start_->right->left = new_node;
    start_->right = new_node;
    size_++;
}

template<typename T>
void List<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("list is empty");
    }
    size_--;
    Node* tmp = end_->left;
    end_->left = tmp->left;
    tmp->left->right = end_;
    delete tmp;
}

// start tmp end

template<typename T>
void List<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("list is empty");
    }
    size_--;
    Node* tmp = start_->right;
    start_->right = tmp->right;
    tmp->right->left = start_;
    delete tmp;
}

template<typename T>
void List<T>::clear() {
    while(!empty()) {
        pop_back();
    }
}

template<typename T>
void List<T>::resize(size_t count) {
    if (count == size_) {
        return;
    }
    else if (count > size_) {
        size_t tmp = size_;
        for (size_t i = 0; i < count - tmp; i++) {
            push_back(T());
        }
    }
    else {
        size_t tmp = size_;
        for (size_t i = 0; i < tmp - count; i++) {
            pop_back();
        }
    }

}

template<typename T>
void List<T>::resize(size_t count, const T& value) {
    if (count == size_) {
        return;
    }
    else if (count > size_) {
        size_t tmp = size_;
        for (size_t i = 0; i < count - tmp; i++) {
            push_back(value);
        }
    }
    else {
        size_t tmp = size_;
        for (size_t i = 0; i < tmp - count; i++) {
            pop_back();
        }
    }

}

// конец и начало связывают все, поэому просто свап их
template<typename T>
void List<T>::swap(List<T>& other) {
    std::swap(size_, other.size_);
    std::swap(start_, other.start_);
    std::swap(end_, other.end_);
}

template<typename T>
List<T>::iterator List<T>::insert(List<T>::iterator pos, const T& value) {
    Node* current_pos = pos.node;
    Node* new_node = new Node(value, current_pos->left, current_pos);
    current_pos->left->right = new_node;
    current_pos->left = new_node;
    size_++;
    return iterator(new_node);

}

template<typename T>
List<T>::iterator List<T>::erase(List<T>::iterator pos) {
    if (size_ == 0) {
        throw std::out_of_range("list is empty");
    }
    Node* current_pos = pos.node;
    iterator ans = ++pos;
    current_pos->left->right = current_pos->right;
    current_pos->right->left = current_pos->left;
    delete current_pos;
    size_--;
    return ans;

}

template <typename T>
std::strong_ordering List<T>::operator<=>(const List<T> &rhs) const {
	return std::lexicographical_compare_three_way(begin(), end(), rhs.begin(), rhs.end());
}

template<typename T>
bool List<T>::operator==(const List<T>& rhs) const {
    return (*this <=> rhs) == std::strong_ordering::equal;
}



}
