#include <iostream>

#include "container.hpp"

namespace my_container {
template <typename T>
class List : public Container<T> {
   protected:
	struct Node {
		T data;
		Node *prev;
		Node *next;

		explicit Node(const T &d, Node *prev = nullptr, Node *next = nullptr);
	};

	size_t size_ = 0;
	Node *head = nullptr;
	Node *tail = nullptr;

   public:
	class iterator {
	   public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T *;
		using reference = T &;
		explicit iterator(Node *node);
		T &operator*();
		T *operator->();
		iterator &operator++();
		iterator operator++(int);
		iterator &operator--();
		iterator operator--(int);
		bool operator==(const iterator &other) const;
		bool operator!=(const iterator &other) const;
		Node *node;
	};

	class const_iterator {
	   public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = const T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T *;
		using reference = const T &;
		const Node *node;
		explicit const_iterator(const Node *node);
		const T &operator*() const;
		const T *operator->() const;
		bool operator==(const const_iterator &other) const;
		bool operator!=(const const_iterator &other) const;
	};

	List();
	List(const List<T> &other);
	List(List<T> &&other) noexcept;
	List(const std::initializer_list<T> &t);
	~List() override;
	List<T> &operator=(const List<T> &other);
	List<T> &operator=(List<T> &&other) noexcept;

	iterator begin() noexcept;
	iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	iterator end() const noexcept;
	const_iterator cend() const noexcept;
	std::reverse_iterator<iterator> rbegin();
	std::reverse_iterator<const_iterator> crbegin();
	std::reverse_iterator<iterator> rend();
	std::reverse_iterator<const_iterator> crend();

	bool empty() const override;
	size_t size() const override;
	size_t max_size() const override;
	void clear();

	iterator insert(iterator pos, const T &value);
	iterator insert(iterator pos, T &&value);
	T &front();
	T &back();
	void push_front(const T &val);
	void push_front(T &&val);
	void push_back(const T &val);
	void push_back(T &&val);
	void pop_back();
	void pop_front();

	iterator erase(iterator pos);
	iterator erase(const_iterator pos);
	iterator erase(iterator first, iterator last);
	iterator erase(const_iterator first, const_iterator last);

	void resize(size_t count, const T &value);
	void resize(size_t count);
	void swap(List<T> &rhs) noexcept;

	std::strong_ordering operator<=>(const List<T> &rhs) const;
	bool operator==(const List<T> &rhs) const;
	bool operator!=(const List<T> &rhs) const;
	bool operator>(const List<T> &rhs) const;
	bool operator>=(const List<T> &rhs) const;
	bool operator<(const List<T> &rhs) const;
	bool operator<=(const List<T> &rhs) const;
	Container<T>& operator=(const Container<T>& other) override;
	bool operator==(const Container<T>& other) const override;
	bool operator!=(const Container<T>& other) const override;
};

template <typename T>
T &List<T>::front() {
	return *(begin());
}

template <typename T>
T &List<T>::back() {
	return *(--end());
}

template <typename T>
void List<T>::swap(List<T> &rhs) noexcept {
	std::swap(head, rhs.head);
	std::swap(tail, rhs.tail);
	std::swap(size_, rhs.size_);
}

template <typename T>
List<T> &List<T>::operator=(List<T> &&other) noexcept {
	if (this != &other) {
		clear();
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(size_, other.size_);
	}
	return *this;
}

template <typename T>
List<T> &List<T>::operator=(const List<T> &other) {
	for (const auto &el : other) {
		push_back(el);
	}
	return *this;
}

template <typename T>
bool List<T>::operator>(const List<T> &rhs) const {
	return (*this <=> rhs) == std::strong_ordering::greater;
}

template <typename T>
bool List<T>::operator>=(const List<T> &rhs) const {
	return (*this <=> rhs) == std::strong_ordering::greater or (*this <=> rhs) == std::strong_ordering::equal;
}

template <typename T>
bool List<T>::operator<(const List<T> &rhs) const {
	return (*this <=> rhs) == std::strong_ordering::less;
}

template <typename T>
bool List<T>::operator<=(const List<T> &rhs) const {
	return (*this <=> rhs) == std::strong_ordering::less or (*this <=> rhs) == std::strong_ordering::equal;
}

template <typename T>
bool List<T>::operator!=(const List<T> &rhs) const {
	return !(*this == rhs);
}

template <typename T>
bool List<T>::operator==(const List<T> &rhs) const {
	return (*this <=> rhs) == std::strong_ordering::equal;
}

template <typename T>
List<T>::List(const std::initializer_list<T> &t) : List() {
	for (auto &elem : t) {
		push_back(elem);
	}
}

template <typename T>
List<T>::List(List<T> &&other) noexcept : List() {
	std::swap(head, other.head);
	std::swap(tail, other.tail);
	std::swap(size_, other.size_);
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const List<T> &list) {
	for (auto &el : list) {
		out << el << " ";
	}
	out << std::endl;
	return out;
}

template <typename T>
std::strong_ordering List<T>::operator<=>(const List<T> &rhs) const {
	return std::lexicographical_compare_three_way(begin(), end(), rhs.begin(), rhs.end());
}

template <typename T>
void List<T>::resize(size_t count) {
	resize(count, 0);
}

template <typename T>
void List<T>::resize(size_t count, const T &value) {
	if (count == size_) {
		return;
	}
	if (count < size_) {
		size_t x = size_ - count;
		for (size_t i = 0; i < x; ++i) {
			pop_back();
		}
	} else if (count > size_) {
		size_t x = count - size_;
		for (size_t i = 0; i < x; ++i) {
			push_back(value);
		}
	}
}

template <typename T>
void List<T>::pop_front() {
	erase(begin());
}

template <typename T>
void List<T>::pop_back() {
	erase(--end());
}

template <typename T>
List<T>::List(const List<T> &other) : List() {
	for (const auto &elem : other) {
		push_back(elem);
	}
}

template <typename T>
List<T>::~List() {
	clear();
	delete head;
	delete tail;
}

template <typename T>
void List<T>::clear() {
	erase(begin(), end());
}

template <typename T>
typename List<T>::iterator List<T>::erase(iterator first, iterator last) {
	if (first == last) {
		return last;
	}
	iterator tmp = first;
	;
	while (tmp != last) {
		tmp = erase(tmp);
	}
	return last;
}

template <typename T>
typename List<T>::iterator List<T>::erase(const_iterator first, const_iterator last) {
	if (first == last) {
		return last;
	}
	iterator tmp = first;
	;
	while (tmp != last) {
		tmp = erase(tmp);
	}
	return last;
}

template <typename T>
typename List<T>::iterator List<T>::erase(List::iterator pos) {
	if (size_ == 0 or pos == end()) {
		throw std::out_of_range("erase from empty list or incorrect position");
	}
	Node *op = pos.node;
	iterator tmp = ++(pos);
	op->prev->next = op->next;
	op->next->prev = op->prev;
	delete op;
	--size_;
	return tmp;
}

template <typename T>
typename List<T>::iterator List<T>::erase(List::const_iterator pos) {
	if (size_ == 0 or pos == end()) {
		throw std::out_of_range("erase from empty list or incorrect position");
	}
	Node *op = pos.node;
	iterator tmp = ++(pos);
	op->prev->next = op->next;
	op->next->prev = op->prev;
	delete op;
	--size_;
	return tmp;
}

template <typename T>
void List<T>::push_back(const T &val) {
	insert(end(), val);
}

template <typename T>
void List<T>::push_back(T &&val) {
	insert(end(), val);
}

template <typename T>
void List<T>::push_front(T &&val) {
	insert(begin(), val);
}

template <typename T>
void List<T>::push_front(const T &val) {
	insert(begin(), val);
}

template <typename T>
size_t List<T>::max_size() const {
	return size_;
}

template <typename T>
typename List<T>::iterator List<T>::insert(List::iterator pos, const T &value) {
	Node *pos_node = pos.node;
	Node *prev_node = pos_node->prev;

	Node *new_node = new Node(value, prev_node, pos_node);
	prev_node->next = new_node;
	pos_node->prev = new_node;

	++size_;
	return iterator(new_node);
}

template <typename T>
typename List<T>::iterator List<T>::insert(List::iterator pos, T &&value) {
	Node *pos_node = pos.node;
	Node *prev_node = pos_node->prev;

	Node *new_node = new Node(value, prev_node, pos_node);
	prev_node->next = new_node;
	pos_node->prev = new_node;

	++size_;
	return iterator(new_node);
}

template <typename T>
size_t List<T>::size() const {
	return size_;
}

template <typename T>
bool List<T>::empty() const {
	return size_ == 0;
}

template <typename T>
std::reverse_iterator<typename my_container::List<T>::const_iterator> List<T>::crbegin() {
	return std::reverse_iterator<const_iterator>(cend());
}

template <typename T>
std::reverse_iterator<typename my_container::List<T>::iterator> List<T>::rbegin() {
	return std::reverse_iterator<iterator>(end());
}

template <typename T>
std::reverse_iterator<typename my_container::List<T>::const_iterator> List<T>::crend() {
	return std::reverse_iterator<const_iterator>(cend());
}

template <typename T>
std::reverse_iterator<typename my_container::List<T>::iterator> List<T>::rend() {
	return std::reverse_iterator<iterator>(end());
}

template <typename T>
List<T>::List() {
	size_ = 0;
	head = new Node(T(), nullptr, nullptr);
	tail = new Node(T(), head, nullptr);
	head->next = tail;
}

template <typename T>
bool List<T>::iterator::operator==(const my_container::List<T>::iterator &other) const {
	return node == other.node;
}

template <typename T>
bool List<T>::iterator::operator!=(const my_container::List<T>::iterator &other) const {
	return node != other.node;
}

template <typename T>
bool List<T>::const_iterator::operator==(const my_container::List<T>::const_iterator &other) const {
	return node == other.node;
}

template <typename T>
bool List<T>::const_iterator::operator!=(const my_container::List<T>::const_iterator &other) const {
	return node != other.node;
}

template <typename T>
typename List<T>::const_iterator List<T>::cend() const noexcept {
	return List::const_iterator{tail};
}

template <typename T>
typename List<T>::iterator List<T>::end() noexcept {
	return List::iterator{tail};
}

template <typename T>
typename List<T>::const_iterator List<T>::cbegin() const noexcept {
	return List::const_iterator{head->next};
}

template <typename T>
typename List<T>::iterator List<T>::begin() noexcept {
	return iterator{head->next};
}

template <typename T>
typename List<T>::iterator List<T>::begin() const noexcept {
	return iterator{head->next};
}

template <typename T>
typename List<T>::iterator List<T>::end() const noexcept {
	return iterator{tail};
}

template <typename T>
List<T>::iterator::iterator(my_container::List<T>::Node *n) : node{n} {}

template <typename T>
List<T>::const_iterator::const_iterator(const my_container::List<T>::Node *n) : node{n} {}

template <typename T>
const T &List<T>::const_iterator::operator*() const {
	return node->data;
}

template <typename T>
const T *my_container::List<T>::const_iterator::operator->() const {
	return &node->data;
}

template <typename T>
T &List<T>::iterator::operator*() {
	return node->data;
}

template <typename T>
T *my_container::List<T>::iterator::operator->() {
	return &node->data;
}

template <typename T>
typename my_container::List<T>::iterator &my_container::List<T>::iterator::operator++() {
	node = node->next;
	return *this;
}

template <typename T>
typename my_container::List<T>::iterator my_container::List<T>::iterator::operator++(int) {
	Node *tmp = *this;
	++(*this);
	return tmp;
}

template <typename T>
typename my_container::List<T>::iterator &my_container::List<T>::iterator::operator--() {
	node = node->prev;
	return *this;
}

template <typename T>
typename my_container::List<T>::iterator my_container::List<T>::iterator::operator--(int) {
	Node *tmp = *this;
	--(*this);
	return tmp;
}

template <typename T>
List<T>::Node::Node(const T &d, List::Node *p, List::Node *n) : data{d}, prev{p}, next{n} {}

	template <typename T>
	Container<T>& List<T>::operator=(const Container<T>& other) {
	const auto& rhs = dynamic_cast<const List<T>&>(other);
	if (this != &rhs) {
		*this = rhs;
	}
	return *this;
	}

	template <typename T>
	bool List<T>::operator==(const Container<T>& other) const {
	const auto& rhs = dynamic_cast<const List<T>&>(other);
	if (size() != rhs.size()) {
		return false;
	}
	return std::equal(begin(), end(), rhs.begin());
	}

	template <typename T>
	bool List<T>::operator!=(const Container<T>& other) const {
	return !(*this == other);
}
}
