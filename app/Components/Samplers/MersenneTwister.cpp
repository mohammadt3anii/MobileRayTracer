//
// Created by Tiago on 19-04-2017.
//

#include "MersenneTwister.hpp"

using ::Components::MersenneTwister;

float MersenneTwister::getSample(const unsigned /*sample*/) noexcept {
    thread_local static ::std::uniform_real_distribution<float> uniform_dist{0.0f, 1.0f};
    thread_local static ::std::mt19937 gen(::std::random_device {}());
    return uniform_dist(gen);
}
