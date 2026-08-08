#ifndef DS_ARRAY_HPP
#define DS_ARRAY_HPP

#include <cstddef>
#include <stdexcept>

namespace ds {

template <typename T>
class DynamicArray
{
private:
    T* data;
    size_t size_;
    size_t capacity_;

    void resize(size_t newCapacity);

public:
    // Constructor & Destructor
    explicit DynamicArray(size_t initialCapacity = 4);
    ~DynamicArray();

    // Disable Copying
    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    // Main Operations
    void push_back(const T& value);
    void pop_back();

    // Element Access
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    // Information
    size_t size() const;
    size_t currentCapacity() const;
    bool empty() const;
};

// ---------------- Constructor ----------------

template <typename T>
DynamicArray<T>::DynamicArray(size_t initialCapacity)
    : capacity_(initialCapacity), size_(0)
{
    data = new T[capacity_];
}

// ---------------- Destructor ----------------

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] data;
}

// ---------------- Resize ----------------

template <typename T>
void DynamicArray<T>::resize(size_t newCapacity)
{
    T* newData = new T[newCapacity];

    for (size_t i = 0; i < size_; i++)
    {
        newData[i] = data[i];
    }

    delete[] data;

    data = newData;
    capacity_ = newCapacity;
}

// ---------------- Push Back ----------------

template <typename T>
void DynamicArray<T>::push_back(const T& value)
{
    if (size_ == capacity_)
    {
        resize(capacity_ * 2);
    }

    data[size_] = value;
    size_++;
}

// ---------------- Pop Back ----------------

template <typename T>
void DynamicArray<T>::pop_back()
{
    if (size_ > 0)
    {
        size_--;
    }
}

// ---------------- Size ----------------

template <typename T>
size_t DynamicArray<T>::size() const
{
    return size_;
}

// ---------------- Capacity ----------------

template <typename T>
size_t DynamicArray<T>::currentCapacity() const
{
    return capacity_;
}

// ---------------- Empty ----------------

template <typename T>
bool DynamicArray<T>::empty() const
{
    return size_ == 0;
}

// ---------------- Operator [] ----------------

template <typename T>
T& DynamicArray<T>::operator[](size_t index)
{
    if (index >= size_)
    {
        throw std::out_of_range("Index out of range");
    }

    return data[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range("Index out of range");
    }

    return data[index];
}

} // namespace ds

#endif