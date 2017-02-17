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

Sampler::~Sampler() {
}

float Sampler::deviation(const unsigned int index) const {
    return (-0.5f + (index * this->deviationIncrement_) - (this->deviationIncrement_ * 0.5f));
}

void Sampler::resetTask() {
    this->task_ = 0;
}

void Sampler::stopRender() {
    this->task_ = this->domainSize_;
    this->samples_ = 0;
    this->maxHalton_ = 0;
}
