//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.hpp"

using MobileRT::Sampler;

Sampler::Sampler(const uint64_t domainSize, const unsigned int samples) noexcept :
  sample_(0),
  domainSize_(domainSize),
  samples_(samples) {
}

Sampler::Sampler(const unsigned int width, const unsigned int height, const unsigned int samples,
                 const unsigned int blockSizeX, const unsigned int blockSizeY) noexcept :
        sample_(0),
        domainSize_((width / blockSizeX) * (height / blockSizeY)),
        samples_(samples) {
}

void Sampler::resetSampling() noexcept {
    this->sample_ = 0ull;
}

void Sampler::stopSampling() noexcept {
    this->samples_ = 0;
}

Sampler::~Sampler () noexcept {
	LOG("SAMPLER DELETED");
}
