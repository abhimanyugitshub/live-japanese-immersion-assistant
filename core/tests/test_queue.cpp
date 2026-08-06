#include <iostream>
#include "../queue.hpp"

int failures = 0;

#define CHECK(condition, description) \
    do { \
        if (condition) { \
            std::cout << "[PASS] " << description << std::endl; \
        } else { \
            std::cout << "[FAIL] " << description << std::endl; \
            ++failures; \
        } \
    } while (0)

void test_basic_fifo_order() {
    ds::Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    CHECK(q.size() == 3, "size is 3 after three enqueues");
    CHECK(q.dequeue() == 1, "first dequeue returns 1 (FIFO order)");
    CHECK(q.dequeue() == 2, "second dequeue returns 2 (FIFO order)");
    CHECK(q.dequeue() == 3, "third dequeue returns 3 (FIFO order)");
    CHECK(q.isEmpty(), "queue is empty after dequeuing everything");
}

void test_peek_does_not_remove() {
    ds::Queue<int> q;
    q.enqueue(42);

    CHECK(q.peek() == 42, "peek returns the front element");
    CHECK(q.size() == 1, "peek does not remove the element");
}

void test_dequeue_on_empty_throws() {
    ds::Queue<int> q;
    bool threw = false;
    try {
        q.dequeue();
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CHECK(threw, "dequeue on an empty queue throws out_of_range");
}

void test_interleaved_enqueue_dequeue() {
    ds::Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    CHECK(q.dequeue() == 1, "interleaved: first dequeue is 1");
    q.enqueue(3);
    CHECK(q.dequeue() == 2, "interleaved: second dequeue is 2");
    CHECK(q.dequeue() == 3, "interleaved: third dequeue is 3");
    CHECK(q.isEmpty(), "interleaved: queue empties correctly");
}

void test_refill_after_empty() {
    ds::Queue<int> q;
    q.enqueue(1);
    q.dequeue();
    CHECK(q.isEmpty(), "queue is empty after single enqueue/dequeue");

    q.enqueue(99);
    CHECK(q.peek() == 99, "queue works correctly after being emptied once");
}

int main() {
    test_basic_fifo_order();
    test_peek_does_not_remove();
    test_dequeue_on_empty_throws();
    test_interleaved_enqueue_dequeue();
    test_refill_after_empty();

    std::cout << std::endl;
    if (failures == 0) {
        std::cout << "All Queue tests passed." << std::endl;
    } else {
        std::cout << failures << " Queue test(s) failed." << std::endl;
    }
    return failures == 0 ? 0 : 1;
}
