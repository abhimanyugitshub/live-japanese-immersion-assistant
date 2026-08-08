#ifndef DS_CIRCULAR_BUFFER_HPP
#define DS_CIRCULAR_BUFFER_HPP

#include <cstddef>
#include <stdexcept>

namespace ds {

template <typename T>
class CircularBuffer
{
private:
    T* data;
    size_t capacity_;
    size_t size_;
    size_t head_;

public:
    // Constructor & Destructor
    explicit CircularBuffer(size_t capacity);
    ~CircularBuffer();

    // Disable Copying
    CircularBuffer(const CircularBuffer&) = delete;
    CircularBuffer& operator=(const CircularBuffer&) = delete;

    // Main Operations
    void push_back(const T& value);
    void pop_front();

    // Information
    size_t size() const;
    size_t currentCapacity() const;
    bool empty() const;

    // Element Access
    const T& operator[](size_t index) const;
};

// ---------------- Constructor ----------------

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t capacity)
    : capacity_(capacity), size_(0), head_(0)
{
    if (capacity_ == 0)
    {
        throw std::invalid_argument("Capacity must be greater than 0");
    }

    data = new T[capacity_];
}

// ---------------- Destructor ----------------

template <typename T>
CircularBuffer<T>::~CircularBuffer()
{
    delete[] data;
}

// ---------------- Push Back ----------------

template <typename T>
void CircularBuffer<T>::push_back(const T& value)
{
    size_t index = (head_ + size_) % capacity_;

    data[index] = value;

    if (size_ < capacity_)
    {
        size_++;
    }
    else
    {
        head_ = (head_ + 1) % capacity_;
    }
}

// ---------------- Pop Front ----------------

template <typename T>
void CircularBuffer<T>::pop_front()
{
    if (size_ > 0)
    {
        head_ = (head_ + 1) % capacity_;
        size_--;
    }
}

// ---------------- Size ----------------

template <typename T>
size_t CircularBuffer<T>::size() const
{
    return size_;
}

// ---------------- Capacity ----------------

template <typename T>
size_t CircularBuffer<T>::currentCapacity() const
{
    return capacity_;
}

// ---------------- Empty ----------------

template <typename T>
bool CircularBuffer<T>::empty() const
{
    return size_ == 0;
}

// ---------------- Operator [] ----------------

template <typename T>
const T& CircularBuffer<T>::operator[](size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range("Index out of range");
    }

    size_t actualIndex = (head_ + index) % capacity_;

    return data[actualIndex];
}

} // namespace ds

#endif