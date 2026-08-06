#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <cstddef>      // std::size_t
#include <stdexcept>    // std::out_of_range, std::runtime_error
#include <utility>      // std::move

// A templated doubly linked list built from raw nodes -- no std::list,
// no std::vector, no std::forward_list. Manual memory management only.
template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;

        explicit Node(const T& val, Node* p = nullptr, Node* n = nullptr)
            : value(val), prev(p), next(n) {}
        explicit Node(T&& val, Node* p = nullptr, Node* n = nullptr)
            : value(std::move(val)), prev(p), next(n) {}
    };

    Node* head_;   // first element
    Node* tail_;   // last element
    std::size_t size_;

    // Deep-copy helper used by copy constructor / copy assignment.
    void copyFrom(const DoublyLinkedList& other) {
        head_ = tail_ = nullptr;
        size_ = 0;
        for (Node* cur = other.head_; cur != nullptr; cur = cur->next) {
            pushBack(cur->value);
        }
    }

    // Locate the node at a given index (0-based). Throws if out of range.
    Node* nodeAt(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("DoublyLinkedList: index out of range");
        }
        Node* cur;
        // Walk from whichever end is closer -- keeps traversal O(n/2) worst case.
        if (index <= size_ / 2) {
            cur = head_;
            for (std::size_t i = 0; i < index; ++i) cur = cur->next;
        } else {
            cur = tail_;
            for (std::size_t i = size_ - 1; i > index; --i) cur = cur->prev;
        }
        return cur;
    }

public:
    // ---- Construction / destruction / copy ----
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    DoublyLinkedList(const DoublyLinkedList& other) { copyFrom(other); }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~DoublyLinkedList() { clear(); }

    // ---- Capacity ----
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // ---- Modifiers ----
    void pushFront(const T& value) {
        Node* n = new Node(value, nullptr, head_);
        if (head_) head_->prev = n;
        head_ = n;
        if (!tail_) tail_ = n;
        ++size_;
    }

    void pushBack(const T& value) {
        Node* n = new Node(value, tail_, nullptr);
        if (tail_) tail_->next = n;
        tail_ = n;
        if (!head_) head_ = n;
        ++size_;
    }

    // Insert value so that it becomes the element at `index`.
    // insert(0, v) behaves like pushFront; insert(size(), v) like pushBack.
    void insert(std::size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("DoublyLinkedList::insert index out of range");
        }
        if (index == 0) { pushFront(value); return; }
        if (index == size_) { pushBack(value); return; }

        Node* at = nodeAt(index);       // node currently occupying `index`
        Node* p = at->prev;
        Node* n = new Node(value, p, at);
        p->next = n;
        at->prev = n;
        ++size_;
    }

    void popFront() {
        if (empty()) throw std::runtime_error("DoublyLinkedList::popFront on empty list");
        Node* old = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr;
        else tail_ = nullptr;
        delete old;
        --size_;
    }

    void popBack() {
        if (empty()) throw std::runtime_error("DoublyLinkedList::popBack on empty list");
        Node* old = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr;
        else head_ = nullptr;
        delete old;
        --size_;
    }

    // Erase element at `index`.
    void erase(std::size_t index) {
        if (index >= size_) throw std::out_of_range("DoublyLinkedList::erase index out of range");
        if (index == 0) { popFront(); return; }
        if (index == size_ - 1) { popBack(); return; }

        Node* target = nodeAt(index);
        target->prev->next = target->next;
        target->next->prev = target->prev;
        delete target;
        --size_;
    }

    // Remove the first occurrence of `value`. Returns true if something was removed.
    bool remove(const T& value) {
        Node* cur = head_;
        std::size_t idx = 0;
        while (cur) {
            if (cur->value == value) {
                erase(idx);
                return true;
            }
            cur = cur->next;
            ++idx;
        }
        return false;
    }

    void clear() {
        Node* cur = head_;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    // ---- Element access ----
    T& front() {
        if (empty()) throw std::runtime_error("DoublyLinkedList::front on empty list");
        return head_->value;
    }
    const T& front() const {
        if (empty()) throw std::runtime_error("DoublyLinkedList::front on empty list");
        return head_->value;
    }

    T& back() {
        if (empty()) throw std::runtime_error("DoublyLinkedList::back on empty list");
        return tail_->value;
    }
    const T& back() const {
        if (empty()) throw std::runtime_error("DoublyLinkedList::back on empty list");
        return tail_->value;
    }

    T& at(std::size_t index) { return nodeAt(index)->value; }
    const T& at(std::size_t index) const { return nodeAt(index)->value; }

    // ---- Bidirectional iterator ----
    class Iterator {
    public:
        explicit Iterator(Node* n) : node_(n) {}

        T& operator*() const { return node_->value; }
        T* operator->() const { return &node_->value; }

        // prefix
        Iterator& operator++() { node_ = node_->next; return *this; }
        Iterator& operator--() { node_ = node_->prev; return *this; }
        // postfix
        Iterator operator++(int) { Iterator tmp = *this; node_ = node_->next; return tmp; }
        Iterator operator--(int) { Iterator tmp = *this; node_ = node_->prev; return tmp; }

        bool operator==(const Iterator& other) const { return node_ == other.node_; }
        bool operator!=(const Iterator& other) const { return node_ != other.node_; }

    private:
        Node* node_;
        friend class DoublyLinkedList;
    };

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }        // one-past-the-end sentinel

    // Use these for backward traversal: for (auto it = rbegin(); it != rend(); --it)
    Iterator rbegin() { return Iterator(tail_); }
    Iterator rend() { return Iterator(nullptr); }        // one-before-the-beginning sentinel
};

#endif // DOUBLY_LINKED_LIST_H
