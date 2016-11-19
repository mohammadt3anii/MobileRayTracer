//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "Scenes/Scene.h"
#include "RayTracer.h"
#include "Cameras/PerspectiveCamera.h"

namespace MobileRT {
    class Renderer {
    private:
        const unsigned int width_;
        const unsigned int height_;
        Scene *scene_;
        RayTracer *rayTracer_;
        PerspectiveCamera *camera_;

    public:
        Renderer(const unsigned int width, const unsigned int height,
                 const unsigned int whichScene, const unsigned int whichShader);

        void render(unsigned int *canvas, const unsigned int numThreads) const;

        void thread_render(unsigned int *canvas, unsigned int tid,
                           unsigned int numThreads) const;
    };
}

#endif //MOBILERAYTRACER_RENDERER_H