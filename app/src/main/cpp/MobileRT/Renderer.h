//
// Created by puscas on 16-10-2016.
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
            const unsigned int RT_W;
            const unsigned int RT_H;
            const unsigned int LowX;
            const unsigned int LowY;
            std::unique_ptr<Scene> scene_;
            std::unique_ptr<RayTrace> rTracer_;
            std::unique_ptr<RTCamera> camera_;

        public:
            Renderer (const unsigned int pcanvasW, const unsigned int pcanvasH, const unsigned int renderRes, const unsigned int whichScene, const unsigned int whichShader);
            void render (uint32_t* canvas, const unsigned int width) const;
    };
}

#endif //MOBILERAYTRACER_RENDERER_H