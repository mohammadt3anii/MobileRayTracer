//
// Created by Tiago on 15-Feb-17.
//

#ifndef MOBILERAYTRACER_DRAWVIEW_H
#define MOBILERAYTRACER_DRAWVIEW_H

#include <thread>
#include <android/bitmap.h>
#include <android/log.h>

#include "../../../../jniLibs/Components/src/main/cpp/MobileRT/Shaders/NoShadows.h"
#include "../../../../jniLibs/Components/src/main/cpp/MobileRT/Shaders/Whitted.h"
#include "../../../../jniLibs/Components/src/main/cpp/MobileRT/Shaders/PathTracer.h"
#include "../../../../jniLibs/Components/src/main/cpp/MobileRT/Samplers/Stratified.h"
#include "../../../../jniLibs/Components/src/main/cpp/MobileRT/Samplers/HaltonSeq.h"
#include "../../../../jniLibs/Components/src/main/cpp/MobileRT/Cameras/Perspective.h"
#include "../../../../jniLibs/Components/src/main/cpp/MobileRT/Lights/PointLight.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Renderer.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Scene.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shapes/Plane.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shapes/Sphere.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shapes/Triangle.h"

void FPS(void);

const MobileRT::Scene *cornellBoxScene(void);

const MobileRT::Scene *spheresScene(void);

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv *,// env,
        jobject//this
);

extern "C"
int Java_puscas_mobilertapp_DrawView_isWorking(
        JNIEnv *,// env,
        jobject//this
);

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *,// env,
        jobject//this
);

extern "C"
void Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv *,// env,
        jobject,//this,
        jint scene,
        jint shader,
        jint width,
        jint height,
        jint sampler,
        jint samples
);

void thread_work(void *dstPixels, unsigned int numThreads);

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap,
        jint nThreads);

extern "C"
int Java_puscas_mobilertapp_DrawView_redraw(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap);

extern "C"
int Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *,
        jobject,
        jfloat jx,
        jfloat jy);

extern "C"
void Java_puscas_mobilertapp_DrawView_moveTouch(
        JNIEnv *,
        jobject,
        jfloat jx,
        jfloat jy,
        jint primitiveIndex
);

extern "C"
float Java_puscas_mobilertapp_DrawView_getFPS(
        JNIEnv *,
        jobject
);

extern "C"
long long Java_puscas_mobilertapp_DrawView_getTimeFrame(
        JNIEnv *,
        jobject
);

#endif //MOBILERAYTRACER_DRAWVIEW_H
