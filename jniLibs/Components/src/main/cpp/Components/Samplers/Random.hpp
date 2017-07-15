//
// Created by Tiago on 19-04-2017.
//

#ifndef COMPONENTS_SAMPLERS_RANDOM_HPP
#define COMPONENTS_SAMPLERS_RANDOM_HPP

#include "MobileRT/Sampler.hpp"
#include <random>

namespace Components {
    class Random final : public MobileRT::Sampler {
    public:
        explicit Random(uint64_t domainSize,
                        unsigned int samples) noexcept;

        float getSample(unsigned int sample) noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SAMPLERS_RANDOM_HPP
