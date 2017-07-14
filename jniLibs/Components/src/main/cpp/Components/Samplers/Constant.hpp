//
// Created by Tiago on 07-04-2017.
//

#ifndef MOBILERAYTRACER_CONSTANT_H
#define MOBILERAYTRACER_CONSTANT_H

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

#endif //MOBILERAYTRACER_STRATIFIED_H
