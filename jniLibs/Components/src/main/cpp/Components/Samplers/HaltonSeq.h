//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_JITTERED_H
#define MOBILERAYTRACER_JITTERED_H

#include "MobileRT/Sampler.h"

namespace Components {
    using namespace MobileRT;

    class HaltonSeq : public Sampler {
    private:
        float haltonSequence(unsigned long long int index, const unsigned int base);

    public:
        explicit HaltonSeq(const unsigned long long int domainSize, const unsigned int samples);

        explicit HaltonSeq(const unsigned int width, const unsigned int height,
                           const unsigned int samples,
                           const unsigned int blockSizeX, const unsigned int blockSizeY);

        virtual float getSample(const unsigned int sample) override;
    };
}

#endif //MOBILERAYTRACER_JITTERED_H
