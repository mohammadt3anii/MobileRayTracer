//
// Created by Tiago on 21-11-2016.
//

#include "HaltonSeq.hpp"

using Components::HaltonSeq;

HaltonSeq::HaltonSeq(const uint64_t domainSize, const unsigned int samples) noexcept :
        Sampler(domainSize, samples) {
}

HaltonSeq::HaltonSeq(const unsigned int width, const unsigned int height,
                     const unsigned int samples,
                     const unsigned int blockSizeX, const unsigned int blockSizeY) noexcept :
        Sampler(width, height, samples, blockSizeX, blockSizeY) {
}

float HaltonSeq::getSample(const unsigned int sample) noexcept {
    const uint64_t current(this->sample_.fetch_add(1ull, std::memory_order_relaxed));
  if (current >= (this->domainSize_ * (sample + 1))) {
        this->sample_.fetch_sub(1ull, std::memory_order_relaxed);
        return 1.0f;
    }
  return haltonSequence(current - (sample * this->domainSize_), 2);
}

//https://en.wikipedia.org/wiki/Halton_sequence
float HaltonSeq::haltonSequence(uint64_t index, const unsigned int base) noexcept {
    float f(1.0f);
    float result(0.0f);
    while (index > 0ull) {
        f /= base;
        result += f * (index % base);
        index = static_cast<uint64_t> (std::floor(index / base));
    }
    return result;
}
