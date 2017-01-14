//
// Created by Tiago on 14-10-2016.
//
#include "MobileRT/Cameras/Perspective.h"
#include "MobileRT/Lights/PointLight.h"
#include "MobileRT/Scene.h"
#include "MobileRT/Renderer.h"
#include "MobileRT/Shapes/Plane.h"
#include "MobileRT/Shapes/Sphere.h"
#include "MobileRT/Shapes/Triangle.h"
#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>

using namespace MobileRT;

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

static int working_(IDLE);
static const MobileRT::Scene *sceneT_(nullptr);
static const MobileRT::Perspective *camera_(nullptr);
static const MobileRT::Renderer *renderer_(nullptr);
static std::thread thread_;

Scene *cornellBoxScene() {
    Scene *scene = new Scene();
    // point light - white
    scene->lights.push_back(new PointLight(RGB(1.0f, 1.0f, 1.0f),
                                           Point3D(0.0f, 0.50f, 0.0f)));

    // back wall - white
    const Material lightGrayMat(RGB(0.9f, 0.9f, 0.9f));
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 0.0f, -1.0f)), lightGrayMat));
    // floor - white
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, -1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)), lightGrayMat));
    // ceiling - white
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, -1.0f, 0.0f)), lightGrayMat));
    // left wall - red
    const Material redMat(RGB(0.9f, 0.0f, 0.0f));
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(-1.0f, 0.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f)), redMat));
    // right wall - blue
    const Material blueMat(RGB(0.0f, 0.0f, 0.9f));
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(1.0f, 0.0f, 0.0f), Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const Material MirrorMat(RGB(0.0f, 0.0f, 0.0f), RGB(0.8f, 0.8f, 0.8f));
    scene->primitives.push_back(new Primitive(new Sphere(
            Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    const Material GreenMat(RGB(0.0f, 0.9f, 0.0f), RGB(0.0f, 0.2f, 0.0f));
    scene->primitives.push_back(new Primitive(new Sphere(
            Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));

    // triangle - yellow
    const Material yellowMat(RGB(1.0f, 1.0f, 0.0f));
    scene->primitives.push_back(new Primitive(new Triangle(
            Point3D(0.5f, -0.5f, 0.99f), Point3D(-0.5f, -0.5f, 0.99f),
            Point3D(0.5f, 0.5f, 1.001f)), yellowMat));
    return scene;
}

Scene *spheresScene() {
    Scene *scene = new Scene();
    // create one light source
    scene->lights.push_back(new PointLight(RGB(1.0f, 1.0f, 1.0f),
                                           Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    Material sandMat(RGB(0.914f, 0.723f, 0.531f));
    Material redMat(RGB(0.9f, 0.0f, 0.0f));
    Material mirrorMat(RGB(0.0f, 0.0f, 0.0f), RGB(0.8f, 0.8f, 0.8f));
    Material greenMat(RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene->primitives.push_back(
            new Primitive(new Sphere(Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene->primitives.push_back(
            new Primitive(new Sphere(Point3D(1.5f, 2.0f, 7.0f), 1.0f), mirrorMat));
    scene->primitives.push_back(
            new Primitive(new Plane(Point3D(0.0f, 0.0f, 0.0f),
                                    Vector3D(0.0f, 1.0f, 0.0f)), sandMat));
    scene->primitives.push_back(
            new Primitive(new Sphere(Point3D(0.0f, 0.5f, 4.5f), 0.5f), greenMat));
    return scene;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_finish(
        JNIEnv *,// env,
        jobject//this
) {
    thread_.join();
    working_ = IDLE;
    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "WORKING = IDLE");
}

extern "C"
int Java_com_example_puscas_mobileraytracer_DrawView_isWorking(
        JNIEnv *,// env,
        jobject//this
) {
    return working_;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_stopRender(
        JNIEnv *,// env,
        jobject//this
) {
    working_ = STOPPED;
    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "WORKING = STOPPED");
    renderer_->stopRender();
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_initialize(
        JNIEnv *,// env,
        jobject,//this,
        jint scene,
        jint shader,
        jint width,
        jint height,
        jint sampler,
        jint samples
) {
    working_ = IDLE;
    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", " ");
    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "WORKING = IDLE");
    const float ratio = static_cast<float>(height) / static_cast<float>(width);
    switch (scene) {
        case 0:
            camera_ = new MobileRT::Perspective(Point3D(0.0f, 0.0f, -3.4f), 45.0f, 45.0f * ratio);
            sceneT_ = cornellBoxScene();
            break;

        case 1:
            camera_ = new MobileRT::Perspective(Point3D(0.0f, 0.5f, 1.0f), 60.0f, 60.0f * ratio);
            sceneT_ = spheresScene();
            break;

        default:
            camera_ = new MobileRT::Perspective(Point3D(0.0f, 0.0f, -3.4f), 45.0f, 45.0f * ratio);
            sceneT_ = cornellBoxScene();
            break;
    }
    renderer_ = new MobileRT::Renderer(
            static_cast<unsigned int>(width),
            static_cast<unsigned int>(height),
            static_cast<unsigned int>(shader),
            static_cast<unsigned int>(sampler),
            *camera_,
            *sceneT_,
            static_cast<unsigned int>(samples)
    );
}

void thread_work(void *dstPixels, unsigned int numThreads) {
    renderer_->render(static_cast<unsigned int *>(dstPixels), numThreads);
    if (working_ != STOPPED) {
        working_ = FINISHED;
        __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "WORKING = FINISHED");
    }
    delete camera_;
    delete sceneT_;
    delete renderer_;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap,
        jint nThreads
) {/*
    const unsigned int number(8);
    std::pair<float, float> limits(limitsHaltonSequence(number));
    const float min = limits.first;
    const float max = limits.second - limits.first;
    const float ratio = (1.0f - limits.second) / 1.5f;
    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Min = %f Max = %f Ratio = %f", min, max,
                        ratio);
    for (unsigned int i (1); i <= number; i++)
    {
        const double value(haltonSequence(i, 2));
        __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG",
                            "Sequence = %f - %d",
                            value,
                            static_cast<unsigned int> (round((value) * number)));
    }*/

    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

    working_ = BUSY;
    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "WORKING = BUSY");
    thread_ = std::thread(thread_work, dstPixels, static_cast<unsigned int> (nThreads));

    AndroidBitmap_unlockPixels(env, dstBitmap);
}