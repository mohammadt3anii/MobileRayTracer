//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_JITTERED_H
#define MOBILERAYTRACER_JITTERED_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class Jittered : public Sampler {
    private:
        float half_rand_max_;
        char padding[4] __attribute__((unused));

    public:
        explicit Jittered(const unsigned int domainSize, const unsigned int samples);

        virtual float getDeviation(const unsigned num) override;

        float getTask(const unsigned int tasks, const unsigned int sample) override;
    };
}

#endif //MOBILERAYTRACER_JITTERED_H
