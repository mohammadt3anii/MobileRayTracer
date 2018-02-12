//
// Created by Tiago on 10-08-2017.
//

#include "StaticHaltonSeq.hpp"
#include <array>

using ::Components::StaticHaltonSeq;
namespace {
    const uint32_t MASK{0xFFFFF};
    const uint32_t SIZE{MASK + 1};
    ::std::array<float, SIZE> VALUES{};

    bool FillThings() {
        static ::std::mt19937 generator(::std::random_device{}());
        for (uint32_t i{0}; i < SIZE; ++i) {
            VALUES.at(i) = ::MobileRT::haltonSequence(i, 2);
        }
        ::std::shuffle(VALUES.begin(), VALUES.end(), generator);
        return true;
    }
}//namespace

StaticHaltonSeq::StaticHaltonSeq() noexcept {
    static bool unused{FillThings()};
    static_cast<void> (unused);
}

StaticHaltonSeq::StaticHaltonSeq(const unsigned width, const unsigned height,
                                 const unsigned samples) noexcept :
        Sampler(width, height, samples) {
    static bool unused{FillThings()};
    static_cast<void> (unused);
}

float StaticHaltonSeq::getSample(const unsigned /*sample*/) noexcept {
    const uint32_t current{this->sample_.fetch_add(1, ::std::memory_order_relaxed)};
    return VALUES.at(current & MASK);
}
