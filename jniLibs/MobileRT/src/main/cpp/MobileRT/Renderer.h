//
// Created by Tiago on 09-Feb-17.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include <thread>
#include "Samplers/Sampler.h"
#include <cstdlib>

namespace MobileRT {
    class Renderer {
    private:
        Sampler &sampler_;
        const Shader &shader_;
        const Camera &camera_;
        unsigned int width_;
        unsigned int height_;
        RGB *const accumulate_;
        unsigned int domainSize_;
        unsigned int blockSize_;
        const unsigned int resolution_;
        char padding[4] __attribute__((unused));

    public:
        explicit Renderer(Sampler &sampler, const Shader &shader, const Camera &camera,
                          const unsigned int width, const unsigned int height);

        ~Renderer(void);

        void renderFrame(unsigned int *const bitmap, const unsigned int numThreads);

        void stopRender(void);

        void renderScene(unsigned int *const bitmap,
                         const unsigned int threadId,
                         const unsigned int numThreads);
    };
}

#endif //MOBILERAYTRACER_RENDERER_H
