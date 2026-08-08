#include "../circular_buffer.hpp"
#include <cassert>
#include <iostream>

int main()
{
    ds::CircularBuffer<int> buffer(4);

    // Test push_back
    buffer.push_back(10);
    buffer.push_back(20);
    buffer.push_back(30);

    assert(buffer.size() == 3);
    assert(buffer[0] == 10);
    assert(buffer[1] == 20);
    assert(buffer[2] == 30);

    // Test pop_front
    buffer.pop_front();

    assert(buffer.size() == 2);
    assert(buffer[0] == 20);
    assert(buffer[1] == 30);

    // Test wraparound
    buffer.push_back(40);
    buffer.push_back(50);
    buffer.push_back(60);

    assert(buffer.size() == 4);
    assert(buffer[0] == 30);
    assert(buffer[1] == 40);
    assert(buffer[2] == 50);
    assert(buffer[3] == 60);

    // Test capacity
    assert(buffer.currentCapacity() == 4);

    // Test empty
    assert(!buffer.empty());

    std::cout << "All CircularBuffer tests passed!\n";

    return 0;
}