//
// Created by Tiago on 21-11-2016.
//

#include "Stratified.h"

using namespace Components;

Stratified::Stratified(const unsigned long long int domainSize, const unsigned int samples) :
        Sampler(domainSize, samples) {
}

Stratified::Stratified(const unsigned int width, const unsigned int height,
                       const unsigned int samples,
                       const unsigned int blockSizeX, const unsigned int blockSizeY) :
        Sampler(width, height, samples, blockSizeX, blockSizeY) {
}

float Stratified::getSample(const unsigned int sample) {
    const unsigned long long int current(this->sample_.fetch_add(1, std::memory_order_relaxed));
    if (isFinished(sample, current)) {
        this->sample_.fetch_sub(1, std::memory_order_relaxed);
        //LOG("current = %llu, domainSize = %llu, sample = %u", current, this->domainSize_, sample);
        return 1.0f;
    }
    const unsigned long long int task(current - (sample * this->domainSize_));
    const float res(static_cast<float> (task) / this->domainSize_);
    return res;
}
