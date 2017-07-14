//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_JITTERED_H
#define MOBILERAYTRACER_JITTERED_H

#include "MobileRT/Sampler.hpp"
#include <cmath>

namespace Components {
    class HaltonSeq final : public MobileRT::Sampler {
    private:
        float haltonSequence(uint64_t index, unsigned int base) noexcept;

    public:
        explicit HaltonSeq(uint64_t domainSize,
                           unsigned int samples) noexcept;

        explicit HaltonSeq(unsigned int width, unsigned int height,
                           unsigned int samples,
                           unsigned int blockSizeX, unsigned int blockSizeY) noexcept;

        float getSample(unsigned int sample) noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_JITTERED_H
