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
        return 1.0f;
    }
    return static_cast<float> (current - (sample * this->domainSize_)) / this->domainSize_;
}
