//
// Created by Tiago on 15-Feb-17.
//

#ifndef MOBILERAYTRACER_DRAWVIEW_H
#define MOBILERAYTRACER_DRAWVIEW_H

#include <thread>
#include <android/bitmap.h>
#include <android/log.h>

#include "MobileRT/Renderer.h"
#include "MobileRT/Scene.h"
#include "MobileRT/Shapes/Plane.h"
#include "MobileRT/Shapes/Sphere.h"
#include "MobileRT/Shapes/Triangle.h"
#include "Components/Shaders/NoShadows.h"
#include "Components/Shaders/Whitted.h"
#include "Components/Shaders/PathTracer.h"
#include "Components/Samplers/Constant.h"
#include "Components/Samplers/Stratified.h"
#include "Components/Samplers/HaltonSeq.h"
#include "Components/Samplers/Random.h"
#include "Components/Cameras/Orthographic.h"
#include "Components/Cameras/Perspective.h"
#include "Components/Lights/PointLight.h"
#include "Components/Lights/AreaLight.h"

static void FPS(void) noexcept;

static MobileRT::Scene *cornellBoxScene(void) noexcept;

static MobileRT::Scene *cornellBoxScene2(void) noexcept;

static MobileRT::Scene *spheresScene(void) noexcept;

static MobileRT::Scene *spheresScene2(void) noexcept;

static void thread_work(void *dstPixels, unsigned int numThreads) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv *,// env,
        jobject//this
) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_isWorking(
        JNIEnv *,// env,
        jobject//this
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *,// env,
        jobject//this
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv *,// env,
        jobject,//this,
        jint scene,
        jint shader,
        jint width,
        jint height,
        jint sampler,
        jint samplesPixel,
        jint samplesLight
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap,
        jint nThreads) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_redraw(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *,
        jobject,
        jfloat jx,
        jfloat jy) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_moveTouch(
        JNIEnv *,
        jobject,
        jfloat jx,
        jfloat jy,
        jint primitiveIndex
) noexcept;

extern "C"
float Java_puscas_mobilertapp_DrawView_getFPS(
        JNIEnv *,
        jobject
) noexcept;

extern "C"
long long Java_puscas_mobilertapp_DrawView_getTimeFrame(
        JNIEnv *,
        jobject
) noexcept;

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_getSample(
        JNIEnv *,
        jobject
) noexcept;

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv *,
        jobject,
        jint size
) noexcept;

#endif //MOBILERAYTRACER_DRAWVIEW_H
