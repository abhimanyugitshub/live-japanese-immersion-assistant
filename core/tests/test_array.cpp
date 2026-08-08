#include "../array.hpp"
#include <cassert>
#include <iostream>

int main()
{
    ds::DynamicArray<int> arr;

    // Test push_back
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    assert(arr.size() == 3);
    assert(arr[0] == 10);
    assert(arr[1] == 20);
    assert(arr[2] == 30);

    // Test resize
    arr.push_back(40);
    arr.push_back(50);

    assert(arr.size() == 5);
    assert(arr.currentCapacity() >= 5);

    // Test pop_back
    arr.pop_back();

    assert(arr.size() == 4);
    assert(arr[3] == 40);

    // Test empty
    assert(!arr.empty());

    std::cout << "All DynamicArray tests passed!\n";

    return 0;
}