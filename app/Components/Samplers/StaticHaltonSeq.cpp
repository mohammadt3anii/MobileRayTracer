//
// Created by Tiago on 10-08-2017.
//

#include "Components/Samplers/StaticHaltonSeq.hpp"
#include <array>

using ::Components::StaticHaltonSeq;
namespace {
    const ::std::uint32_t MASK{0xFFFFF};
    const ::std::uint32_t SIZE{MASK + 1};
    ::std::array<float, SIZE> VALUES{};

    bool FillThings() {
        static ::std::mt19937 generator(::std::random_device{}());
        for (::std::uint32_t i{0}; i < SIZE; ++i) {
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

StaticHaltonSeq::StaticHaltonSeq(const ::std::uint32_t width, const ::std::uint32_t height,
                                 const ::std::uint32_t samples) noexcept :
        Sampler(width, height, samples) {
    static bool unused{FillThings()};
    static_cast<void> (unused);
}

float StaticHaltonSeq::getSample(const ::std::uint32_t /*sample*/) noexcept {
    const ::std::uint32_t current {this->sample_.fetch_add(1, ::std::memory_order_relaxed)};
    const float res{VALUES.at(current & MASK)};
    return res;
}
