//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"

using namespace MobileRT;

Sampler::Sampler(const unsigned long long int domainSize, const unsigned int samples) :
        sample_(0),
        overSample_(0),
        domainSize_(domainSize),
        samples_(static_cast<const unsigned int> (std::sqrt(samples))),
        maxHalton_(roundToUpperPower2(domainSize * samples)),
        deviationIncrement_(1.0f / samples) {
}

Sampler::Sampler(const unsigned int width, const unsigned int height, const unsigned int samples,
                 const unsigned int blockSizeX, const unsigned int blockSizeY) :
        sample_(0),
        overSample_(0),
        domainSize_(
                roundToEvenNumber(width) / roundToMultipleOf(blockSizeX, roundToEvenNumber(width)) *
                roundToEvenNumber(height) /
                roundToMultipleOf(blockSizeY, roundToEvenNumber(height))),
        samples_(static_cast<const unsigned int> (std::sqrt(samples))),
        maxHalton_(roundToUpperPower2(
                roundToEvenNumber(width) / roundToMultipleOf(blockSizeX, roundToEvenNumber(width)) *
                roundToEvenNumber(height) /
                roundToMultipleOf(blockSizeY, roundToEvenNumber(height)) * samples)),
        deviationIncrement_(1.0f / samples) {
}

Sampler::~Sampler(void) {
}

bool Sampler::notFinished(const unsigned int sample) {
    return (((this->sample_ - 1) - this->overSample_) < this->domainSize_ * (sample + 1));
}

void Sampler::resetSampling() {
    this->sample_ = 0;
    this->overSample_ = 0;
}

void Sampler::stopRender(void) {
    this->overSample_ = 0;
    this->samples_ = 0;
    this->maxHalton_ = 0;
}
