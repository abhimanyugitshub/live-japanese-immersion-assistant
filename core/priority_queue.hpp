#ifndef DS_PRIORITY_QUEUE_HPP
#define DS_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace ds {

// Default comparator gives a MIN-heap (smaller value = higher priority).
// Pass a different Compare to get a max-heap or a custom ordering
// (e.g. comparing by a struct's "priority" field).
template <typename T>
struct Less {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

// Binary heap implemented over a raw, manually-resized array.
// No STL containers used — no std::vector, no std::priority_queue.
template <typename T, typename Compare = Less<T>>
class PriorityQueue {
private:
    T* data;
    size_t capacity;
    size_t count;
    Compare comp;

    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < count; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t leftChild(size_t i) const { return 2 * i + 1; }
    size_t rightChild(size_t i) const { return 2 * i + 2; }

    void siftUp(size_t i) {
        while (i > 0 && comp(data[i], data[parent(i)])) {
            std::swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }

    void siftDown(size_t i) {
        while (true) {
            size_t left = leftChild(i);
            size_t right = rightChild(i);
            size_t highest = i;

            if (left < count && comp(data[left], data[highest])) {
                highest = left;
            }
            if (right < count && comp(data[right], data[highest])) {
                highest = right;
            }
            if (highest == i) {
                break;
            }
            std::swap(data[i], data[highest]);
            i = highest;
        }
    }

public:
    explicit PriorityQueue(size_t initialCapacity = 16)
        : capacity(initialCapacity == 0 ? 1 : initialCapacity), count(0) {
        data = new T[capacity];
    }

    ~PriorityQueue() {
        delete[] data;
    }

    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    void insert(const T& value) {
        if (count == capacity) {
            resize(capacity * 2);
        }
        data[count] = value;
        siftUp(count);
        ++count;
    }

    // Removes and returns the highest-priority element
    // (smallest, under the default Less comparator).
    T extract() {
        if (count == 0) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        T top = data[0];
        --count;
        data[0] = data[count];
        siftDown(0);

        // Shrink if usage drops well below capacity, to avoid wasting memory.
        if (count > 0 && count <= capacity / 4 && capacity > 16) {
            resize(capacity / 2);
        }
        return top;
    }

    const T& top() const {
        if (count == 0) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        return data[0];
    }

    bool isEmpty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

    // Confirms the heap property holds for every parent/child pair.
    // Useful for tests, not needed in normal operation.
    bool isValidHeap() const {
        for (size_t i = 0; i < count; ++i) {
            size_t left = leftChild(i);
            size_t right = rightChild(i);
            if (left < count && comp(data[left], data[i])) {
                return false;
            }
            if (right < count && comp(data[right], data[i])) {
                return false;
            }
        }
        return true;
    }
};

} // namespace ds

#endif // DS_PRIORITY_QUEUE_HPP
