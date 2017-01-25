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

namespace MobileRT {
    class Sampler {
    private:
        virtual void renderScene(unsigned int *const bitmap,
                                 const unsigned int threadId,
                                 const unsigned int numThreads) = 0;

    protected:
        unsigned int width_;
        unsigned int height_;
        const Shader &shader_;
        const Camera &camera_;
        const unsigned int samples_;
        const float deviationIncrement_;
        std::atomic<unsigned int> taskLine_;
        RGB *const accumulate_;
        unsigned int tasks_;
        unsigned int maxHalton_;

    public:
        Sampler(const unsigned int width, const unsigned int height,
                const Shader &shader, const unsigned int samples,
                const Camera &camera);
        virtual ~Sampler();

        const float deviation(const int index) const;

        void resetTask();

        const unsigned int getTasks(const unsigned int tasks, const int sample);

        void stopRender();

        void renderFrame(unsigned int *const bitmap, const unsigned int numThreads);
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H