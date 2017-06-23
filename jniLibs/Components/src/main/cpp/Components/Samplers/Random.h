//
// Created by Tiago on 19-04-2017.
//

#ifndef MOBILERAYTRACER_RANDOM_H
#define MOBILERAYTRACER_RANDOM_H

#include "MobileRT/Sampler.h"
#include <cstdlib>

namespace Components {
    using namespace MobileRT;

    class Random final : public Sampler {
    public:
        explicit Random(const unsigned long long int domainSize, const unsigned int samples);

        virtual float getSample(const unsigned int sample) override final;
    };
}

#endif //MOBILERAYTRACER_RANDOM_H
