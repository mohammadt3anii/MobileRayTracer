//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"

using namespace MobileRT;

Sampler::Sampler(const unsigned long long int domainSize, const unsigned int samples) noexcept :
        sample_(0ull),
        domainSize_(domainSize),
        samples_(samples) {
}

Sampler::Sampler(const unsigned int width, const unsigned int height, const unsigned int samples,
                 const unsigned int blockSizeX, const unsigned int blockSizeY) noexcept :
        sample_(0ull),
        domainSize_((width / blockSizeX) * (height / blockSizeY)),
        samples_(samples) {
}

Sampler::~Sampler(void) noexcept {
}

void Sampler::resetSampling() noexcept {
    this->sample_ = 0ull;
}

void Sampler::stopSampling(void) noexcept {
    this->samples_ = 0ull;
}
