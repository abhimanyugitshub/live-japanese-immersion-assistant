#include <cassert>
#include <iostream>
#include "../audio_window.hpp"

int main()
{
    // Small window of 5 samples
    ds::AudioWindow w(5);

    float a[3] = {1, 2, 3};
    w.push(a, 3);
    assert(w.size() == 3);
    auto s1 = w.snapshot();
    assert(s1.size() == 3 && s1[0] == 1 && s1[2] == 3);

    // Push 4 more (total 7, capacity 5) -> oldest two are overwritten
    float b[4] = {4, 5, 6, 7};
    w.push(b, 4);
    assert(w.size() == 5);
    assert(w.totalPushed() == 7);
    auto s2 = w.snapshot();
    assert(s2[0] == 3 && s2[1] == 4 && s2[4] == 7);

    // Stress: 1,000,000 samples into a 10-second window (16 kHz)
    ds::AudioWindow big(160000);
    float chunk[1600] = {0};
    for (int i = 0; i < 625; ++i)
    {
        big.push(chunk, 1600);
    }
    assert(big.size() == 160000);
    assert(big.totalPushed() == 1000000);

    std::cout << "All AudioWindow tests passed\n";
    return 0;
}