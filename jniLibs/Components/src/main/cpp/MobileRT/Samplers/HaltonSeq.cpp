//
// Created by Tiago on 21-11-2016.
//

#include "HaltonSeq.h"

using namespace MobileRT;

HaltonSeq::HaltonSeq(const unsigned long long int domainSize, const unsigned int samples,
                    const bool boolean) :
        Sampler(domainSize, samples),
        half_rand_max_(RAND_MAX * 0.5f)
{
        this->maxSampler_ = 1.0f;
        boolean_ = boolean;
}

HaltonSeq::HaltonSeq(const unsigned long long int domainSize, const unsigned int samples) :
        Sampler(domainSize, samples),
        half_rand_max_(RAND_MAX * 0.5f)
{
        this->maxSampler_ = 1.0f;
        boolean_ = false;
}

HaltonSeq::HaltonSeq(const unsigned int width, const unsigned int height,
                     const unsigned int samples,
                     const unsigned int blockSizeX, const unsigned int blockSizeY) :
        Sampler(width, height, samples, blockSizeX, blockSizeY),
        half_rand_max_(RAND_MAX * 0.5f)
{
    this->maxSampler_ = (static_cast<float> ((domainSize_ * samples_)) / maxHalton_);
    boolean_ = false;
}

float HaltonSeq::getDeviation(const unsigned) {
    return std::rand() / half_rand_max_ - 1.0f;
}

float HaltonSeq::getSample(const unsigned int sample) {
    if (!notFinished(sample)) return 1.0f;
    const unsigned int aux(static_cast<unsigned int>(
                                   this->sample_.fetch_add(1, std::memory_order_relaxed)));
    const unsigned int task(static_cast<unsigned int> (aux - (sample * this->domainSize_)));
    const float res(haltonSequence(task, 2));
    if (res >= this->maxSampler_) {
        this->overSample_.fetch_add(1, std::memory_order_relaxed);
        //LOG("res = %f [%f]", double(res), double(maxSampler_));
    }
    return res;
}

//https://en.wikipedia.org/wiki/Halton_sequence
float HaltonSeq::haltonSequence(const unsigned int index, const unsigned int base) {
    float f(1.0f);
    float result(0.0f);
    float i(index);
    while (i > 0.0f) {
        f = f / base;
        result = result + f * (static_cast<unsigned int> (i) % base);
        i = std::floor(i / base);
    }
    return result;
}
