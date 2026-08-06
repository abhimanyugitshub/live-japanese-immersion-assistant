#ifndef DS_QUEUE_HPP
#define DS_QUEUE_HPP

#include <cstddef>
#include <stdexcept>

namespace ds {

// Templated FIFO queue built on a custom doubly linked list.
// No STL containers used — raw pointers and manual memory management only.
template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        explicit Node(const T& value)
            : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head; // front of the queue (dequeue happens here)
    Node* tail; // back of the queue (enqueue happens here)
    size_t count;

public:
    Queue() : head(nullptr), tail(nullptr), count(0) {}

    ~Queue() {
        clear();
    }

    // No implicit copies — matches the HashTable's ownership pattern.
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& value) {
        Node* node = new Node(value);
        if (tail == nullptr) {
            // empty queue
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        ++count;
    }

    T dequeue() {
        if (head == nullptr) {
            throw std::out_of_range("Queue is empty");
        }
        Node* old = head;
        T value = old->data;

        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            // queue is now empty
            tail = nullptr;
        }

        delete old;
        --count;
        return value;
    }

    T& peek() const {
        if (head == nullptr) {
            throw std::out_of_range("Queue is empty");
        }
        return head->data;
    }

    bool isEmpty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }
};

} // namespace ds

#endif // DS_QUEUE_HPP
