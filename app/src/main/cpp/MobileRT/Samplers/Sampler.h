//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_SAMPLER_H
#define MOBILERAYTRACER_SAMPLER_H

#include "../Utils.h"
#include "../RayTracer.h"
#include "../Cameras/Camera.h"
#include "../RGB.h"
#include <atomic>
#include <thread>

namespace MobileRT {
    class Sampler {
    protected:
        unsigned int width_;
        unsigned int height_;
        const RayTracer &rayTracer_;
        const Camera &camera_;
        const unsigned int samples_;
        const float deviationIncrement_;
        std::atomic<unsigned int> taskLine_;
        RGB *const accumulate_;
        unsigned int tasks_;
        unsigned int maxHalton_;

    public:
        Sampler(const unsigned int width, const unsigned int height,
                const RayTracer &rayTracer, const unsigned int samples,
                const Camera &camera);
        virtual ~Sampler();

        virtual void renderScene(unsigned int *canvas,
                                 const unsigned int threadId,
                                 const unsigned int numThreads) = 0;

        const float deviation(const int index) const;

        void resetTask();

        const unsigned int getTasks(const unsigned int tasks, const int sample);

        void stopRender();

        void renderFrame(unsigned int *const bitmap, const unsigned int numThreads);
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H