//
// Created by Tiago on 10-08-2017.
//

#ifndef COMPONENTS_SAMPLERS_STATICHALTONSEQ_HPP
#define COMPONENTS_SAMPLERS_STATICHALTONSEQ_HPP

#include "MobileRT/Sampler.hpp"
#include <algorithm>
#include <random>

namespace Components {
    class StaticHaltonSeq final : public ::MobileRT::Sampler {
    public:
        explicit StaticHaltonSeq() noexcept;

        explicit StaticHaltonSeq(unsigned width, unsigned height,
                                 unsigned samples) noexcept;

        StaticHaltonSeq(const StaticHaltonSeq &random) noexcept = delete;

        StaticHaltonSeq(StaticHaltonSeq &&random) noexcept = delete;

        ~StaticHaltonSeq() noexcept final = default;

        StaticHaltonSeq &operator=(const StaticHaltonSeq &random) noexcept = delete;

        StaticHaltonSeq &operator=(StaticHaltonSeq &&random) noexcept = delete;

        float getSample(unsigned sample) noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SAMPLERS_STATICHALTONSEQ_HPP
