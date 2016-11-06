//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "Point.h"
#include "Scene.h"
#include "RayTrace.h"
#include "RTCamera.h"
#include "ToneMapper.h"
#include <array>

namespace MobileRT {
    class Renderer {
    private:
        float fov;
        int RT_W, RT_H;
        int LowX=-0, LowY=0;
        std::unique_ptr<Scene> scene_;
        std::unique_ptr<RayTrace> rTracer_;
        std::unique_ptr<RTCamera> camera_;

    public:
        Renderer (int pcanvasW, int pcanvasH, int renderRes, int whichScene, int whichShader);
        void render (uint32_t* canvas, int width, int height);
    };
}

#endif //MOBILERAYTRACER_RENDERER_H