//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "Scenes/Scene.h"
#include "RayTracer.h"
#include "Cameras/Perspective.h"
#include "Samplers/Sampler.h"

namespace MobileRT {
    class Renderer {
    private:
        Sampler *sampler_;

    public:
        Renderer(const unsigned int width, const unsigned int height,
                 const unsigned int whichScene, const unsigned int whichShader,
                 const unsigned int whichSampler, const unsigned int samples = 1);

        void render(unsigned int *canvas, const unsigned int numThreads) const;
    };
}

#endif //MOBILERAYTRACER_RENDERER_H