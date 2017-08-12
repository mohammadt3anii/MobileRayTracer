//
// Created by Tiago on 10-08-2017.
//

#include "StaticHaltonSeq.hpp"

using Components::StaticHaltonSeq;
namespace {
  static const uint32_t SIZE {0xFFFFF};
  static std::array<float, SIZE> VALUES;

  static bool FillThings () {
    for (uint32_t i {0}; i < SIZE; ++i) {
      VALUES[i] = haltonSequence (i, 2);
    }
    std::random_shuffle (VALUES.begin (), VALUES.end ());
    return true;
  }
}

StaticHaltonSeq::StaticHaltonSeq () noexcept {
  static bool unused {FillThings ()};
}

StaticHaltonSeq::StaticHaltonSeq (const unsigned int width, const unsigned int height,
                                  const unsigned int samples) noexcept :
  Sampler (width, height, samples) {
  static bool unused {FillThings ()};
}

float StaticHaltonSeq::getSample (const unsigned int /*sample*/) noexcept {
  const uint32_t current {this->sample_.fetch_add (1, std::memory_order_relaxed)};
  return VALUES[current & SIZE];
}
