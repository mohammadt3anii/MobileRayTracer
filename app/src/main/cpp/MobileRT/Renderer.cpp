//
// Created by puscas on 16-10-2016.
//

#include "Renderer.h"
#include "SceneCornell.h"
#include "SceneSpheres.h"
#include <android/log.h>
#include "ToneMapper.h"

using namespace MobileRT;

Renderer::Renderer (int pcanvasW, int pcanvasH, int renderRes, int whichScene, int whichShader) {
    float vfov;
    RT_H = RT_W = renderRes;
    LowX = (pcanvasW-RT_W) >> 1;
    LowY = (pcanvasH-RT_H) >> 1;

    // create and load the Scene, parameterize the camera
    switch (whichScene) {
        case 0 : // cornell
            mScene = new SceneCornell();
            fov = 45.f;
            From=new myPoint(0.f, 0.f, -3.4f);
            break;
        case 1 : // spheres
            mScene = new SceneSpheres();
            fov=60.f;
            From=new myPoint(0.f, .5f, 1.f);
            break;
    }

    // create the ray tracer
    mRTracer = new RayTrace(*mScene, whichShader);

    // create the camera
    vfov = fov * (((float)RT_H) / ((float)RT_W));
    mCamera = new RTCamera (From, fov, vfov);

    mToneMapper = new ToneMapper();

}

void Renderer::render (uint32_t* canvas, int width, int height){
    float INV_IMG_WIDTH = 1.0f / RT_W;
    float INV_IMG_HEIGHT = 1.0f / RT_H;
    for(int y=0; y<RT_H; y++) {
        for (int x = 0; x < RT_W; x++) {
            // generate the ray
            float u = (float) x * INV_IMG_WIDTH;
            float v = (float) y * INV_IMG_HEIGHT;
            Ray* r = mCamera->getRay (u,v);
            RGB* rayRGB = mRTracer->RayV(*r);

            // tonemap and convert to Paint
            //p = mToneMapper.RGB2Color (rayRGB);

            unsigned char cr = static_cast<uint8_t>(rayRGB->R*255);
            unsigned char cg = static_cast<uint8_t>(rayRGB->G*255);
            unsigned char cb = static_cast<uint8_t>(rayRGB->B*255);
            canvas[x + y*width] = 0xFF000000 | (cb << 16) | (cg << 8) | cr;
            //__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Need to print : %02x %02x %02x %08x\n", cr, cg, cb, canvas[x + y*width]);
        }
    }
}