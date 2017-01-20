//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "Samplers/Sampler.h"
#include <thread>

namespace MobileRT {
    class Renderer {
    private:
        Sampler *const sampler_;

    public:
        Renderer(Sampler &sampler);

        ~Renderer();

        void renderFrame(unsigned int *const bitmap, const unsigned int numThreads) const;

        void stopRender() const;
    };
}

#endif //MOBILERAYTRACER_RENDERER_H