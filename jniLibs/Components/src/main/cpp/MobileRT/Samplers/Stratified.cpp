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
    if (this->domainSize_ <= 1) return 0;
    const unsigned int task(static_cast<unsigned int> (
                                    this->sample_.fetch_add(1, std::memory_order_relaxed) -
                                    (sample * this->domainSize_)));
    const float res(static_cast<float> (task) / this->domainSize_);
    /*ASSERT(res, >=, 0);
    ASSERT(res, <=, 1.0f);
    if (task > this->domainSize_) raise(SIGTRAP);*/
    return res;
}
