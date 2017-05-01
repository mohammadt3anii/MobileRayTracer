//
// Created by Tiago on 19-04-2017.
//

#ifndef MOBILERAYTRACER_RANDOM_H
#define MOBILERAYTRACER_RANDOM_H

#include "MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class Random : public Sampler {
    public:
        explicit Random(const unsigned long long int domainSize, const unsigned int samples);

        virtual float getSample(const unsigned int sample) override;
    };
}

#endif //MOBILERAYTRACER_RANDOM_H
