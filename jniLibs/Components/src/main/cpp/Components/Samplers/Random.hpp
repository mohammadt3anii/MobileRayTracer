//
// Created by Tiago on 19-04-2017.
//

#ifndef MOBILERAYTRACER_RANDOM_H
#define MOBILERAYTRACER_RANDOM_H

#include "MobileRT/Sampler.hpp"
#include <cstdlib>
#include <random>

namespace Components {
    class Random final : public MobileRT::Sampler {
    public:
        explicit Random(uint64_t domainSize,
                        unsigned int samples) noexcept;

        float getSample(unsigned int sample) noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_RANDOM_H
