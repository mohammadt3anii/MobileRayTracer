//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_SAMPLER_H
#define MOBILERAYTRACER_SAMPLER_H

#include "../Utils.h"
#include "../Shaders/Shader.h"
#include "../Cameras/Camera.h"
#include "../RGB.h"
#include <atomic>
#include <thread>
#include <cstdlib>

namespace MobileRT {
    class Sampler {
    public:
        const unsigned int domainSize_;
        unsigned int samples_;
        unsigned int maxHalton_;
        const float deviationIncrement_;
        std::atomic<unsigned int> task_;
        char padding[4] __attribute__((unused));

    public:
        explicit Sampler(const unsigned int domainSize, const unsigned int samples);

        virtual ~Sampler(void);

        float deviation(const unsigned int index) const;

        void resetTask(const unsigned int value);

        void stopRender(void);

        bool notFinished(void);

        virtual float getDeviation(const unsigned int num) = 0;

        virtual float getTask(const unsigned int tasks, const unsigned int sample) = 0;
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H
