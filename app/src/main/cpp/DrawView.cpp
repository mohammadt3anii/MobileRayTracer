//
// Created by puscas on 14-10-2016.
//

#include "DrawView.h"

#include <jni.h>
#include <android/bitmap.h>
#include "MobileRT/myPoint.h"

/**
 * Draws something into the given bitmap
 * @param  env
 * @param  thiz
 * @param  dstBitmap   The bitmap to place the results
 * @param  width       The bitmap with
 * @param  height      The bitmap height
 * @param  elapsedTime The number of milliseconds since the app was started
 */
extern "C"
/*JNIEXPORT*/ void /*JNICALL*/ Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv* env,
        jobject /*thiz*/,
        jobject dstBitmap,
        jint width,
        jint height,
        jlong /*elapsedTime*/)
{
    // Grab the dst bitmap info and pixels
    AndroidBitmapInfo dstInfo;
    void* dstPixels;
    AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

    unsigned char * rgb = static_cast<unsigned char*>(dstPixels);
    auto size = width*height*4;
    auto start = width*height*4/2;


    /*if(sceneComplete_)
    {

    }
    else
    {

    }*/

    for(auto i = start; i < size; i+=4)
    {
        rgb[i] = 255;
        rgb[i+1] = 255;
        rgb[i+2] = 255;
        rgb[i+3] = 255;
    }

    // Unlock the dst's pixels
    AndroidBitmap_unlockPixels(env, dstBitmap);
}


DrawView::DrawView() : sceneComplete_(false)
{

   /* Renderer (int pcanvasW, int pcanvasH, int renderRes, int whichScene, int whichShader) {
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
        mRTracer = new RayTrace(mScene, whichShader);

        // create the camera
        vfov = fov * (((float)RT_H) / ((float)RT_W));
        mCamera = new RTCamera (From, fov, vfov);

        mToneMapper = new ToneMapper();

    }*/

}