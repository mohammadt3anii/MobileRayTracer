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
        unsigned int domainSize_;
        unsigned int samples_;
        unsigned int maxHalton_;
        const float deviationIncrement_;
        std::atomic<unsigned int> sample_;
        std::atomic<unsigned int> overSample_;
        //char padding[4] __attribute__((unused));

    public:
        explicit Sampler(const unsigned int domainSize, const unsigned int samples);

        explicit Sampler(const unsigned int width, const unsigned height,
                         const unsigned int samples,
                         const unsigned int blockSizeX, const unsigned int blockSizeY);

        virtual ~Sampler(void);

        void resetSampling(void);

        void stopRender(void);

        bool notFinished(const unsigned int sample);

        virtual float getDeviation(const unsigned int num) = 0;

        virtual float getSample(const unsigned int tasks, const unsigned int sample) = 0;
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H
