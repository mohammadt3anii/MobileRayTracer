//
// Created by Tiago on 19-04-2017.
//

#include "Random.h"

using namespace Components;

Random::Random(const unsigned long long int domainSize, const unsigned int samples) noexcept :
        Sampler(domainSize, samples) {
}

float Random::getSample(const unsigned int) noexcept {
    return static_cast<float> (rand()) / RAND_MAX;
}
