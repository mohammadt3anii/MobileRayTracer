//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "Scene.h"
#include "RayTracer.h"
#include "Cameras/Perspective.h"
#include "Samplers/Sampler.h"
#include <thread>

namespace MobileRT {
    class Renderer {
    private:
        Sampler *const sampler_;

    public:
        Renderer(const Shader &shader, Sampler &sampler,
                 const Perspective &camera, const Scene &scene, const unsigned int samples = 1);

        ~Renderer();

        void render(unsigned int *canvas, const unsigned int numThreads) const;

        void stopRender() const;
    };
}

#endif //MOBILERAYTRACER_RENDERER_H