//
// Created by Tiago on 10-08-2017.
//

#include "StaticMersenneTwister.hpp"

using Components::StaticMersenneTwister;
namespace {
  static const uint32_t SIZE {0xFFFFF};
  static std::array<float, SIZE> VALUES;

  static bool FillThings () {
    static std::uniform_real_distribution<float> uniform_dist {0.0f, 1.0f};
    static std::mt19937 gen (std::random_device {} ());
    for (uint32_t i {0}; i < SIZE; ++i) {
      VALUES[i] = uniform_dist (gen);
    }
    return true;
  }
}

StaticMersenneTwister::StaticMersenneTwister () noexcept {
  static bool unused {FillThings ()};
}

float StaticMersenneTwister::getSample (const unsigned int /*sample*/) noexcept {
  const uint32_t current {this->sample_.fetch_add (1, std::memory_order_relaxed)};
  return VALUES[current & SIZE];
}
