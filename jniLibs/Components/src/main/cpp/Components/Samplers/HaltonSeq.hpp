//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_JITTERED_H
#define MOBILERAYTRACER_JITTERED_H

#include <cmath>
#include "MobileRT/Sampler.hpp"

namespace Components {
    class HaltonSeq final : public MobileRT::Sampler {
    private:
        float haltonSequence(unsigned long long int index, const unsigned int base) noexcept;

    public:
        explicit HaltonSeq(unsigned long long int domainSize,
                           unsigned int samples) noexcept;

        explicit HaltonSeq(unsigned int width, unsigned int height,
                           unsigned int samples,
                           unsigned int blockSizeX, unsigned int blockSizeY) noexcept;

        virtual float getSample(const unsigned int sample) noexcept override final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_JITTERED_H
