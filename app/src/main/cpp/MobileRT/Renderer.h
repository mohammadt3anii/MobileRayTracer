//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "myPoint.h"
#include "Scene.h"
#include "RayTrace.h"
#include "RTCamera.h"
#include "ToneMapper.h"
#include <array>

namespace MobileRT {
    class Renderer {
    private:
        float fov;
        myPoint* From;
        int RT_W, RT_H;
        int LowX=-0, LowY=0;
        Scene* mScene;
        RayTrace* mRTracer;
        RTCamera* mCamera;
        ToneMapper* mToneMapper;

    public:
        Renderer (int pcanvasW, int pcanvasH, int renderRes, int whichScene, int whichShader);
        void render (unsigned char* c, int RT_width, int RT_height, int xmin, int ymin,
                               int xincrement,int yincrement);
    };
}

#endif //MOBILERAYTRACER_RENDERER_H