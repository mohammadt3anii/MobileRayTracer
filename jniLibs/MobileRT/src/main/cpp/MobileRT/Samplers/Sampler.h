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
        std::atomic<unsigned long long int> sample_;
        std::atomic<unsigned long long int> overSample_;
        unsigned long long int domainSize_;
        unsigned long long int samples_;
        unsigned long long int maxHalton_;
        const float deviationIncrement_;

    public:
        explicit Sampler(const unsigned long long int domainSize, const unsigned int samples);

        explicit Sampler(const unsigned int width, const unsigned int height,
                         const unsigned int samples,
                         const unsigned int blockSizeX, const unsigned int blockSizeY);

        virtual ~Sampler(void);

        void resetSampling(void);

        void stopRender(void);

        bool notFinished(const unsigned int sample);

        virtual float getDeviation(const unsigned int num) = 0;

        virtual float getSample(const unsigned int sample) = 0;
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H
