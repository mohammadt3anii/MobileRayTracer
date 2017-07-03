//
// Created by Tiago on 07-04-2017.
//

#ifndef MOBILERAYTRACER_CONSTANT_H
#define MOBILERAYTRACER_CONSTANT_H

#include "MobileRT/Sampler.h"

namespace Components {
    using namespace MobileRT;

    class Constant final : public Sampler {
    private:
        const float value_;

    public:
        explicit Constant(const float value);

        virtual float getSample(const unsigned int sample) override final;
    };
}

#endif //MOBILERAYTRACER_STRATIFIED_H
