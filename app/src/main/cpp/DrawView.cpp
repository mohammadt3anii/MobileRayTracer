//
// Created by puscas on 14-10-2016.
//

#include "DrawView.h"

#include <math.h>
#include <jni.h>
#include <android/bitmap.h>

/**
 * Draws something into the given bitmap
 * @param  env
 * @param  thiz
 * @param  dstBitmap   The bitmap to place the results of skia into
 * @param  elapsedTime The number of milliseconds since the app was started
 */
extern "C"
JNIEXPORT void JNICALL Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv* env,
        jobject thiz,
        jobject dstBitmap,
        jlong elapsedTime)
{
    // Grab the dst bitmap info and pixels
    AndroidBitmapInfo dstInfo;
    void* dstPixels;
    AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

    unsigned char * rgb = (unsigned char * )dstPixels;


    for(auto i = 0; i < 1000000; i+=3)
    {
        rgb[i] = 255;
    }

    /*SkImageInfo info = SkImageInfo::MakeN32Premul(dstInfo.width, dstInfo.height);

    // Create a surface from the given bitmap
    sk_sp<SkSurface> surface(SkSurface::MakeRasterDirect(info, dstPixels, dstInfo.stride));
    SkCanvas* canvas = surface->getCanvas();

    // Draw something "interesting"

    // Clear the canvas with a white color
    canvas->drawColor(SK_ColorWHITE);

    // Adapt the SkPaint for drawing blue lines
    paint.setAntiAlias(false); // Turning off anti-aliasing speeds up the line drawing
    paint.setColor(0xFF0000FF); // This is a solid blue color for our lines
    paint.setStrokeWidth(SkIntToScalar(2)); // This makes the lines have a thickness of 2 pixels

    // Draw some interesting lines using trig functions
    for (int i = 0; i < 100; i++)
    {
        float x = (float)i / 99.0f;
        float offset = elapsedTime / 1000.0f;
        canvas->drawLine(sin(x * M_PI + offset) * 800.0f, 0,   // first endpoint
                         cos(x * M_PI + offset) * 800.0f, 800, // second endpoint
                         paint);                               // SkPapint to tell how to draw the line
    }*/

    // Unlock the dst's pixels
    AndroidBitmap_unlockPixels(env, dstBitmap);
}
