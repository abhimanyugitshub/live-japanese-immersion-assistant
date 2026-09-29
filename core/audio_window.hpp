#ifndef DS_AUDIO_WINDOW_HPP
#define DS_AUDIO_WINDOW_HPP

#include <cstddef>
#include <vector>
#include "circular_buffer.hpp"

namespace ds {

// Keeps only the most recent N audio samples (a "rolling window").
class AudioWindow
{
private:
    CircularBuffer<float> buffer_;   // your Week 1 buffer
    size_t totalPushed_;             // how many samples we ever received

public:
    explicit AudioWindow(size_t windowSamples)
        : buffer_(windowSamples), totalPushed_(0) {}

    // Add new audio. When full, the oldest samples are overwritten.
    void push(const float* data, size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            buffer_.push_back(data[i]);
        }
        totalPushed_ += count;
    }

    // Copy the whole window out, oldest sample first.
    std::vector<float> snapshot() const
    {
        std::vector<float> out(buffer_.size());
        for (size_t i = 0; i < out.size(); ++i)
        {
            out[i] = buffer_[i];     // index 0 = oldest
        }
        return out;
    }

    size_t size() const { return buffer_.size(); }
    size_t capacity() const { return buffer_.currentCapacity(); }
    size_t totalPushed() const { return totalPushed_; }
};

} // namespace ds

#endif