//
// Created by puscas on 14-10-2016.
//

#include "DrawView.h"

#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <unistd.h>

/**
 * Draws something into the given bitmap
 * @param  env
 * @param  thiz
 * @param  dstBitmap   The bitmap to place the results
 * @param  width       The bitmap with
 * @param  height      The bitmap height
 * @param  elapsedTime The number of milliseconds since the app was started
 */

unsigned char* canvas;
int mCanvasWidth =  1;
int mCanvasHeight = 1;
int whichScene = 0;
int whichShader = 0;
int mRenderRes = 500;
long size;
long start;
int x = 0;
Renderer* r = new Renderer(mCanvasWidth, mCanvasHeight, mRenderRes, whichScene, whichShader);

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv* env,
        jobject /*thiz*/,
        jobject dstBitmap,
        jint width,
        jint height,
        jlong /*elapsedTime*/) {

    // Grab the dst bitmap info and pixels
    AndroidBitmapInfo dstInfo;
    void* dstPixels;
    size = width*height*4;
    start = width*height*4/2;

    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
    canvas = static_cast<unsigned char*>(dstPixels);
    for(auto i = start; i < size; i+=4) {
        canvas[i] = 255;
        canvas[i+1] = 0;
        canvas[i+2] = 0;
        canvas[i+3] = 255;
    }
    int RT_W = width;
    int RT_H = height;
    int horizontal_blocks = 5;
    int vertical_blocks = 5;
    for(int y = 0; y<RT_H; y+=RT_H/vertical_blocks) {
        for (x = 0; x < RT_W; x += RT_W / horizontal_blocks) {
            r->render(canvas, RT_W, RT_H, x, y, RT_W / horizontal_blocks, RT_H / vertical_blocks);
        }
    }
    // Unlock the dst's pixels
    AndroidBitmap_unlockPixels(env, dstBitmap);
}