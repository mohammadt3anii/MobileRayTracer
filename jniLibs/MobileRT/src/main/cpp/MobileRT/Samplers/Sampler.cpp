//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"

using namespace MobileRT;

Sampler::Sampler(const unsigned int domainSize, const unsigned int samples) :
        domainSize_(domainSize),
        samples_(static_cast<const unsigned int> (std::sqrt(samples))),
        maxHalton_(roundToPower2(domainSize * samples)),
        deviationIncrement_(1.0f / samples),
        task_(0) {
}

Sampler::~Sampler(void) {
}

bool Sampler::notFinished(const unsigned int sample) {
    return this->task_ < this->domainSize_ * (sample + 1);
}

float Sampler::deviation(const unsigned int index) const {
    return (-0.5f + (index * this->deviationIncrement_) - (this->deviationIncrement_ * 0.5f));
}

void Sampler::resetTask(const unsigned int value) {
    this->task_ = value;
}

void Sampler::stopRender(void) {
    this->task_ = this->domainSize_ * this->samples_;
    this->domainSize_ = 0;
    this->samples_ = 0;
    this->maxHalton_ = 0;
}
