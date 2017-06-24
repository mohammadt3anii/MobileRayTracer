//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"

using namespace MobileRT;

Sampler::Sampler(const unsigned long long int domainSize, const unsigned int samples) :
        sample_(0),
        domainSize_(domainSize),
        samples_(samples) {
}

Sampler::Sampler(const unsigned int width, const unsigned int height, const unsigned int samples,
                 const unsigned int blockSizeX, const unsigned int blockSizeY) :
        sample_(0),
        domainSize_((width / blockSizeX) * (height / blockSizeY)),
        samples_(samples) {
}

Sampler::~Sampler(void) {
}

bool Sampler::isFinished(const unsigned int sample, const unsigned long long int current) {
    return (current >= (this->domainSize_ * (sample + 1u)));
}

void Sampler::resetSampling() {
    this->sample_ = 0ull;
}

void Sampler::stopSampling(void) {
    this->samples_ = 0ull;
}
