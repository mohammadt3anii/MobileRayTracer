//
// Created by puscas on 14-10-2016.
//

#include "DrawViewImpl.h"
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

static Renderer* r;

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawViewImpl_initialize(
        JNIEnv *,// env,
        jobject, //thiz,
        jint whichScene,
        jint whichShader,
        jint width,
        jint height
) {
    r = new Renderer(width, height, width > height ? height : width, whichScene, whichShader);
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawViewImpl_drawIntoBitmap(
        JNIEnv* env,
        jobject, //thiz,
        jobject dstBitmap,
        jint width
) {

    // Grab the dst bitmap info and pixels
    //AndroidBitmapInfo dstInfo;
    void* dstPixels;

    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    //AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);

    // draw scene
    r->render(static_cast<uint32_t *>(dstPixels), width);

    // Unlock the dst's pixels
    //AndroidBitmap_unlockPixels(env, dstBitmap);
}