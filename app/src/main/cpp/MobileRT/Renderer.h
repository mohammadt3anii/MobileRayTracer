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
        float fov;
        int RT_W, RT_H;
        int LowX=-0, LowY=0;
        std::unique_ptr<Scene> scene_;
        std::unique_ptr<RayTrace> rTracer_;
        std::unique_ptr<RTCamera> camera_;

    public:
        Renderer (const int pcanvasW, const int pcanvasH, const int renderRes, const int whichScene, const int whichShader);
        void render (uint32_t* canvas, const int width);
    };
}

#endif //MOBILERAYTRACER_RENDERER_H