//
// Created by Tiago on 21-11-2016.
//

#include "Stratified.h"

using namespace MobileRT;

Stratified::Stratified(const unsigned int domainSize, const unsigned int samples) :
        Sampler(domainSize, samples) {
}

Stratified::Stratified(const unsigned int width, const unsigned int height,
                       const unsigned int samples,
                       const unsigned int blockSizeX, const unsigned int blockSizeY) :
        Sampler(width, height, samples, blockSizeX, blockSizeY) {
}

float Stratified::getDeviation(const unsigned int index) {
    return (-0.5f + (index * this->deviationIncrement_) - (this->deviationIncrement_ * 0.5f));
}

float Stratified::getSample(const unsigned int tasks, const unsigned int sample) {
    unsigned int task(this->sample_.fetch_add(tasks, std::memory_order_relaxed));
    return static_cast<float> (task - (sample * this->domainSize_)) / this->domainSize_;
}
