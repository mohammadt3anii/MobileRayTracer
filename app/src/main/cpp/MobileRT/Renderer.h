//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "Scene.h"
#include "RayTrace.h"
#include "RTCamera.h"

namespace MobileRT
{
    class Renderer
    {
        private:
        const unsigned int width_;
        const unsigned int height_;
            Scene* scene_;
            RayTrace* rTracer_;
            RTCamera* camera_;

        public:
        Renderer(const unsigned int width, const unsigned int height, const unsigned int whichScene,
                 const unsigned int whichShader);

        void render(unsigned int *canvas, const unsigned int numThreads) const;

        void call_from_thread(unsigned int *canvas, unsigned int startY, unsigned int stopY) const;
    };
}

#endif //MOBILERAYTRACER_RENDERER_H