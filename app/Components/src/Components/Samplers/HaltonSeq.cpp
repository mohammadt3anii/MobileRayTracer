//
// Created by Tiago on 21-11-2016.
//

#include "HaltonSeq.hpp"

using ::Components::HaltonSeq;

HaltonSeq::HaltonSeq(const unsigned width, const unsigned height,
                     const unsigned samples) noexcept :
        Sampler(width, height, samples) {
}

float HaltonSeq::getSample(const unsigned sample) noexcept {
    const uint32_t current{this->sample_.fetch_add(1, ::std::memory_order_relaxed)};
    if (current >= (this->domainSize_ * (sample + 1))) {
        this->sample_.fetch_sub(1, ::std::memory_order_relaxed);
        return 1.0f;
    }
    return ::MobileRT::haltonSequence(current - (sample * this->domainSize_), 2);
}
