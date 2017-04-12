//
// Created by Tiago on 07-04-2017.
//

#ifndef MOBILERAYTRACER_CONSTANT_H
#define MOBILERAYTRACER_CONSTANT_H


#include "MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class Constant : public Sampler {
    private:
        const float value_;

    public:
        explicit Constant(const float value);

        float getSample(const unsigned int sample) override;
    };
}

#endif //MOBILERAYTRACER_STRATIFIED_H
