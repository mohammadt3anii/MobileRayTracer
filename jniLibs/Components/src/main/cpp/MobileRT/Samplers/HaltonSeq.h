//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_JITTERED_H
#define MOBILERAYTRACER_JITTERED_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class HaltonSeq : public Sampler {
    private:
        float half_rand_max_;
        float maxSampler_;
        //char padding[4] __attribute__((unused));

    private:
        float haltonSequence(const unsigned int index, const unsigned int base);

    public:
        explicit HaltonSeq(const unsigned int domainSize, const unsigned int samples);

        explicit HaltonSeq(const unsigned int width, const unsigned int height,
                           const unsigned int samples,
                           const unsigned int blockSizeX, const unsigned int blockSizeY);

        virtual float getDeviation(const unsigned num) override;

        float getSample(const unsigned int tasks, const unsigned int sample) override;
    };
}

#endif //MOBILERAYTRACER_JITTERED_H
