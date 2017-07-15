//
// Created by Tiago on 07-04-2017.
//

#ifndef COMPONENTS_SAMPLERS_CONSTANT_HPP
#define COMPONENTS_SAMPLERS_CONSTANT_HPP

#include "MobileRT/Sampler.hpp"

namespace Components {
    class Constant final : public MobileRT::Sampler {
    private:
        const float value_;

    public:
        explicit Constant(float value) noexcept;

        float getSample(unsigned int sample) noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SAMPLERS_CONSTANT_HPP
