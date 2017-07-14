//
// Created by Tiago on 15-Feb-17.
//

#ifndef MOBILERAYTRACER_DRAWVIEW_H
#define MOBILERAYTRACER_DRAWVIEW_H

#include <thread>
#include <android/bitmap.h>
#include <android/log.h>

#include "MobileRT/Renderer.hpp"
#include "MobileRT/Scene.hpp"
#include "MobileRT/Shapes/Plane.hpp"
#include "MobileRT/Shapes/Sphere.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include "MobileRT/Shapes/Rectangle.hpp"
#include "Components/Shaders/NoShadows.hpp"
#include "Components/Shaders/Whitted.hpp"
#include "Components/Shaders/PathTracer.hpp"
#include "Components/Samplers/Constant.hpp"
#include "Components/Samplers/Stratified.hpp"
#include "Components/Samplers/HaltonSeq.hpp"
#include "Components/Samplers/Random.hpp"
#include "Components/Cameras/Orthographic.hpp"
#include "Components/Cameras/Perspective.hpp"
#include "Components/Lights/PointLight.hpp"
#include "Components/Lights/AreaLight.hpp"

static void FPS(void) noexcept;

static MobileRT::Scene *cornellBoxScene(void) noexcept;

static MobileRT::Scene *cornellBoxScene2(void) noexcept;

static MobileRT::Scene *spheresScene(void) noexcept;

static MobileRT::Scene *spheresScene2(void) noexcept;

static void thread_work(void *dstPixels, const unsigned int numThreads) noexcept;

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
        const JNIEnv *,// env,
        const jobject,//this,
        const jint scene,
        const jint shader,
        const jint width,
        const jint height,
        const jint sampler,
        const jint samplesPixel,
        const jint samplesLight
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
