//
// Created by Tiago on 10-08-2017.
//

#ifndef COMPONENTS_SAMPLERS_STATICHALTONSEQ_HPP
#define COMPONENTS_SAMPLERS_STATICHALTONSEQ_HPP

#include "MobileRT/Sampler.hpp"
#include <algorithm>
#include <array>
#include <random>

namespace Components {
  class StaticHaltonSeq final : public MobileRT::Sampler {
    public:
    explicit StaticHaltonSeq () noexcept;
    explicit StaticHaltonSeq (unsigned int width, unsigned int height,
                              unsigned int samples) noexcept;
    StaticHaltonSeq (const StaticHaltonSeq &random) noexcept = delete;
    StaticHaltonSeq (StaticHaltonSeq &&random) noexcept = delete;
    ~StaticHaltonSeq () noexcept final = default;
    StaticHaltonSeq &operator= (const StaticHaltonSeq &random) noexcept = delete;
    StaticHaltonSeq &operator= (StaticHaltonSeq &&random) noexcept = delete;
    float getSample (unsigned int sample) noexcept final;
  };
}//namespace Components

#endif //COMPONENTS_SAMPLERS_STATICHALTONSEQ_HPP
