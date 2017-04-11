//
// Created by Tiago on 21-11-2016.
//

#include "Stratified.h"

using namespace MobileRT;

Stratified::Stratified(const unsigned long long int domainSize, const unsigned int samples) :
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

float Stratified::getSample(const unsigned int sample) {
    if (!notFinished(sample)) return 1.0f;
    const unsigned int aux(static_cast<unsigned int> (
                                   this->sample_.fetch_add(1, std::memory_order_relaxed)));
    const unsigned int task(static_cast<unsigned int> (aux - (sample * this->domainSize_)));
    const float res(static_cast<float> (task) / this->domainSize_);
    return res;
}
