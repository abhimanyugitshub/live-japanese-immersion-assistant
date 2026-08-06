#include <iostream>
#include "../priority_queue.hpp"

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

void test_min_heap_extract_order() {
    ds::PriorityQueue<int> pq; // default Less<T> -> min-heap
    pq.insert(5);
    pq.insert(1);
    pq.insert(3);
    pq.insert(4);
    pq.insert(2);

    CHECK(pq.size() == 5, "size is 5 after five inserts");
    CHECK(pq.extract() == 1, "extract returns smallest value first (1)");
    CHECK(pq.extract() == 2, "extract returns next smallest (2)");
    CHECK(pq.extract() == 3, "extract returns next smallest (3)");
    CHECK(pq.extract() == 4, "extract returns next smallest (4)");
    CHECK(pq.extract() == 5, "extract returns next smallest (5)");
    CHECK(pq.isEmpty(), "priority queue is empty after extracting everything");
}

// Custom comparator for a max-heap.
struct Greater {
    bool operator()(int a, int b) const {
        return a > b;
    }
};

void test_max_heap_with_custom_comparator() {
    ds::PriorityQueue<int, Greater> pq;
    pq.insert(5);
    pq.insert(1);
    pq.insert(9);
    pq.insert(3);

    CHECK(pq.extract() == 9, "max-heap: extract returns largest value first (9)");
    CHECK(pq.extract() == 5, "max-heap: extract returns next largest (5)");
    CHECK(pq.isValidHeap(), "max-heap: heap property holds after extracts");
}

void test_heap_property_after_many_operations() {
    ds::PriorityQueue<int> pq;
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13, 2, 9, 5, 11, 12, 0};
    for (int v : values) {
        pq.insert(v);
        CHECK(pq.isValidHeap(), "heap property holds after each insert");
    }

    int prev = pq.extract();
    bool ordered = true;
    while (!pq.isEmpty()) {
        int next = pq.extract();
        if (next < prev) {
            ordered = false;
        }
        prev = next;
    }
    CHECK(ordered, "repeated extracts come out in non-decreasing order");
}

void test_extract_on_empty_throws() {
    ds::PriorityQueue<int> pq;
    bool threw = false;
    try {
        pq.extract();
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CHECK(threw, "extract on an empty priority queue throws out_of_range");
}

void test_grows_past_initial_capacity() {
    ds::PriorityQueue<int> pq(2); // tiny initial capacity, forces resize
    for (int i = 20; i > 0; --i) {
        pq.insert(i);
    }
    CHECK(pq.size() == 20, "priority queue grows correctly past its initial capacity");
    CHECK(pq.extract() == 1, "smallest value still correct after growth");
}

int main() {
    test_min_heap_extract_order();
    test_max_heap_with_custom_comparator();
    test_heap_property_after_many_operations();
    test_extract_on_empty_throws();
    test_grows_past_initial_capacity();

    std::cout << std::endl;
    if (failures == 0) {
        std::cout << "All PriorityQueue tests passed." << std::endl;
    } else {
        std::cout << failures << " PriorityQueue test(s) failed." << std::endl;
    }
    return failures == 0 ? 0 : 1;
}
