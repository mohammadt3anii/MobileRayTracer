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

void Renderer::render (unsigned char* c, int RT_width, int RT_height, int xmin, int ymin,
                       int xincrement,int yincrement){
    //Paint p;
    float u, v;
    RGB* rayRGB;
    float INV_IMG_HEIGHT = 1.f / (float) RT_height;
    float INV_IMG_WIDTH = 1.f / (float) RT_width;

    for(int y=ymin; y<(ymin+yincrement) && y< RT_height; y++) {
        for (int x = xmin; x < (xmin+xincrement) && x < RT_width; x++) {
            // generate the ray
            u = (float) x * INV_IMG_WIDTH;
            v = (float) y * INV_IMG_HEIGHT;
            //__android_log_print(ANDROID_LOG_DEBUG, "LOG", "1 (x,y)=(%d,%d)\n", x, y);fflush(stdout);
            Ray* r = mCamera->getRay (u,v);
            /*__android_log_print(ANDROID_LOG_DEBUG, "ORIG", "(x,y,z)=(%f,%f,%f) \n", r->orig->x, r->orig->y, r->orig->z);fflush(stdout);
            __android_log_print(ANDROID_LOG_DEBUG, "DIR", "(x,y,z)=(%f,%f,%f) \n", r->dir->x, r->dir->y, r->dir->z);fflush(stdout);*/
            //__android_log_print(ANDROID_LOG_DEBUG, "LOG", "2 (x,y)=(%d,%d)\n", x, y);fflush(stdout);

            /*if ((x==150) && (y==150))
                rayRGB = mRTracer->RayV(*r);
            else rayRGB = mRTracer->RayV(*r);*/

            // tonemap and convert to Paint
            //p = mToneMapper.RGB2Color (rayRGB);

            /*c[x + LowX + (y + LowY)*4 + 0] = rayRGB->R;
            c[x + LowX + (y + LowY)*4 + 1] = rayRGB->G;
            c[x + LowX + (y + LowY)*4 + 2] = rayRGB->B;*/
        }
    }
}