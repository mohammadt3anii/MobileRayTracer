//
// Created by Tiago on 15-Feb-17.
//

#ifndef MOBILERAYTRACER_DRAWVIEW_H
#define MOBILERAYTRACER_DRAWVIEW_H

#include <thread>
#include <android/bitmap.h>
#include <android/log.h>

#define LOG(msg, ...)\
__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "line:%d: " msg, __LINE__, __VA_ARGS__);

#include "Shaders/NoShadows.h"
#include "Shaders/Whitted.h"
#include "Samplers/Stratified.h"
#include "Samplers/Jittered.h"
#include "Cameras/Perspective.h"
#include "Lights/PointLight.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Renderer.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Scene.h"
#include "Shapes/Plane.h"
#include "Shapes/Sphere.h"
#include "Shapes/Triangle.h"

void FPS();

MobileRT::Scene *cornellBoxScene();

const MobileRT::Scene *spheresScene();

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
