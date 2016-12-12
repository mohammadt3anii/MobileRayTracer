//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_SAMPLER_H
#define MOBILERAYTRACER_SAMPLER_H

#include "../Scene.h"
#include "../RayTracer.h"
#include "../Cameras/Perspective.h"
#include <atomic>

namespace MobileRT {
    class Sampler {
    protected:
        unsigned int width_;
        unsigned int height_;
        const RayTracer *rayTracer_;
        const Perspective *camera_;
        const unsigned int samples_;
        const float deviationIncrement_;
        std::atomic<unsigned int> taskLine_;
        RGB *accumulate_;
        unsigned int tasks_;
        unsigned int maxHalton_;

    public:
        Sampler(const unsigned int width, const unsigned int height,
                const unsigned int shader, const unsigned int samples,
                const Perspective &camera, const Scene &scene);
        virtual ~Sampler();

        virtual void renderScene(unsigned int *canvas) = 0;

        const float deviation(const int index) const;

        void resetTask();

        const unsigned int getTask(const int sample);

        void stopSampling();
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H