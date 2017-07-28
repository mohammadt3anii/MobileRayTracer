//
// Created by Tiago on 15-Feb-17.
//

#ifndef APP_DRAWVIEW_HPP
#define APP_DRAWVIEW_HPP

#include "Components/Cameras/Orthographic.hpp"
#include "Components/Cameras/Perspective.hpp"
#include "Components/Lights/AreaLight.hpp"
#include "Components/Lights/PointLight.hpp"
#include "Components/Samplers/Constant.hpp"
#include "Components/Samplers/HaltonSeq.hpp"
#include "Components/Samplers/Random.hpp"
#include "Components/Samplers/Stratified.hpp"
#include "Components/Shaders/DepthMap.hpp"
#include "Components/Shaders/NoShadows.hpp"
#include "Components/Shaders/PathTracer.hpp"
#include "Components/Shaders/Whitted.hpp"
#include "MobileRT/Renderer.hpp"
#include "MobileRT/Scene.hpp"
#include "MobileRT/Shapes/Plane.hpp"
#include "MobileRT/Shapes/Rectangle.hpp"
#include "MobileRT/Shapes/Sphere.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include <android/bitmap.h>
#include <android/log.h>
#include <thread>

static void FPS() noexcept;

static MobileRT::Scene cornellBoxScene(MobileRT::Scene&& scene) noexcept;

static MobileRT::Scene cornellBoxScene2(MobileRT::Scene&& scene) noexcept;

static MobileRT::Scene spheresScene(MobileRT::Scene&& scene) noexcept;

static MobileRT::Scene spheresScene2(MobileRT::Scene&& scene) noexcept;

static void thread_work(void *dstPixels, unsigned int numThreads) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_isWorking(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv *env,
        jobject thiz,
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
        jobject thiz,
        jobject dstBitmap,
        jint nThreads) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_redraw(
        JNIEnv *env,
        jobject thiz,
        jobject dstBitmap) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *env,
        jobject thiz,
        jfloat jx,
        jfloat jy) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_moveTouch(
        JNIEnv *env,
        jobject thiz,
        jfloat jx,
        jfloat jy,
        jint primitiveIndex
) noexcept;

extern "C"
float Java_puscas_mobilertapp_DrawView_getFPS(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
int64_t Java_puscas_mobilertapp_DrawView_getTimeFrame(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_getSample(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv * /*env*/,
        jobject /*thiz*/,
        jint size
) noexcept;

#endif //APP_DRAWVIEW_HPP
