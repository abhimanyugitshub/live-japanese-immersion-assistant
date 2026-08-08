#ifndef STACK_H
#define STACK_H

#include "DoublyLinkedList.h"
#include <stdexcept>

// Stack<T> implemented on top of DoublyLinkedList<T> (composition, not
// inheritance -- the stack "has a" list, it isn't a list with restrictions
// bolted on). Push/pop/peek run in O(1) since they operate on the list's tail.
template <typename T>
class Stack {
private:
    DoublyLinkedList<T> data_;

public:
    Stack() = default;

    void push(const T& value) {
        data_.pushBack(value);
    }

    void pop() {
        if (empty()) throw std::runtime_error("Stack::pop on empty stack");
        data_.popBack();
    }

    T& peek() {
        if (empty()) throw std::runtime_error("Stack::peek on empty stack");
        return data_.back();
    }

    const T& peek() const {
        if (empty()) throw std::runtime_error("Stack::peek on empty stack");
        return data_.back();
    }

    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }
};

#endif // STACK_H
