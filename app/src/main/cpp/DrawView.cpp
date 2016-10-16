//
// Created by puscas on 14-10-2016.
//

//#include "DrawView.h"

#include <jni.h>
#include <android/bitmap.h>

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
