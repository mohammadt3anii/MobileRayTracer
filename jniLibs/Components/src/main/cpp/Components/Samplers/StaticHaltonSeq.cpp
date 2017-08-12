//
// Created by Tiago on 10-08-2017.
//

#include "StaticHaltonSeq.hpp"

using Components::StaticHaltonSeq;
namespace {
  const uint32_t SIZE {0xFFFFF};
  std::array<float, SIZE> VALUES;

  bool FillThings () {
    static std::mt19937 generator (std::random_device{} ());
    for (uint32_t i {0}; i < SIZE; ++i) {
      VALUES.at (i) = haltonSequence (i, 2);
    }
    std::shuffle (VALUES.begin (), VALUES.end (), generator);
    return true;
  }
}//namespace

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
  return VALUES.at (current & SIZE);
}
