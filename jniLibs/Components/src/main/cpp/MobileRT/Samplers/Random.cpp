//
// Created by Tiago on 19-04-2017.
//

#include "Random.h"

using namespace MobileRT;

Random::Random(const unsigned long long int domainSize, const unsigned int samples) :
        Sampler(domainSize, samples)
{
}

float Random::getSample(const unsigned int) {
    const float res (static_cast<float>(rand()) / (RAND_MAX));
    return res;
}
