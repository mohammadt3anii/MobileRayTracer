//
// Created by Tiago on 14-10-2016.
//
#include "MobileRT/Shaders/NoShadows.h"
#include "MobileRT/Shaders/Whitted.h"
#include "MobileRT/Samplers/Stratified.h"
#include "MobileRT/Samplers/Jittered.h"
#include "MobileRT/Shapes/Plane.h"
#include "MobileRT/Shapes/Sphere.h"
#include "MobileRT/Shapes/Triangle.h"
#include "MobileRT/Cameras/Perspective.h"
#include "MobileRT/Lights/PointLight.h"
#include <android/bitmap.h>
#include <android/log.h>

#define LOG(msg, ...)\
__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "line:%d: " msg, __LINE__, __VA_ARGS__);

using namespace MobileRT;

enum State {
    IDLE = 0, BUSY = 1, FINISH = 2, FINISHED = 3, STOPPED = 4
};

static int working_(IDLE);
static const MobileRT::Scene *scene_(nullptr);
static const MobileRT::Perspective *camera_(nullptr);
static const MobileRT::RayTracer *rayTracer_(nullptr);
static MobileRT::Sampler *sampler_(nullptr);
static MobileRT::Shader *shader_(nullptr);
static std::thread thread_;
static unsigned int width_(0);
static unsigned int height_(0);
static std::chrono::steady_clock::time_point timebase_;
static float fps_(0);

void FPS() {
    static int frame(0);
    frame++;
    const std::chrono::steady_clock::time_point time(std::chrono::steady_clock::now());
    if (std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count() > 1000) {
        fps_ = (frame * 1000.0f) /
               (std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count());
        timebase_ = time;
        frame = 0;
    }
}

Scene *const cornellBoxScene() {
    Scene *const scene = new Scene();
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

const Scene *const spheresScene() {
    Scene *const scene = new Scene();
    // create one light source
    scene->lights.push_back(new PointLight(RGB(1.0f, 1.0f, 1.0f),
                                           Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const Material sandMat(RGB(0.914f, 0.723f, 0.531f));
    const Material redMat(RGB(0.9f, 0.0f, 0.0f));
    const Material mirrorMat(RGB(0.0f, 0.0f, 0.0f), RGB(0.8f, 0.8f, 0.8f));
    const Material greenMat(RGB(0.0f, 0.9f, 0.0f));
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
void Java_com_example_puscas_mobileraytracer_DrawView_clearStage(
        JNIEnv *,// env,
        jobject//this
) {
    working_ = FINISHED;
    LOG("%s", "WORKING = FINISHED");
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_finish(
        JNIEnv *,// env,
        jobject//this
) {
    thread_.join();
    working_ = IDLE;
    LOG("%s", "WORKING = IDLE");
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
    LOG("%s", "WORKING = STOPPED");
    sampler_->stopRender();
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
    LOG("%s", "WORKING = IDLE");
    const float ratio(static_cast<float>(height) / static_cast<float>(width));
    width_ = static_cast<unsigned int>(width);
    height_ = static_cast<unsigned int>(height);

    const unsigned int samples_(static_cast<unsigned int>(samples));
    switch (scene) {
        case 1:
            camera_ = new MobileRT::Perspective(Point3D(0.0f, 0.5f, 1.0f), 60.0f, 60.0f * ratio);
            scene_ = spheresScene();
            break;

        default:
            camera_ = new MobileRT::Perspective(Point3D(0.0f, 0.0f, -3.4f), 45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene();
            break;
    }
    switch (shader) {
        case 1:
            shader_ = new MobileRT::Whitted(*scene_);
            break;

        default:
            shader_ = new MobileRT::NoShadows(*scene_);
            break;
    }
    rayTracer_ = new RayTracer(*scene_, *shader_);
    switch (sampler) {
        case 1 :
            sampler_ = new Jittered(width_, height_, *rayTracer_, samples_, *camera_);
            break;

        default:
            sampler_ = new Stratified(width_, height_, *rayTracer_, samples_, *camera_);
            break;
    }
}

void thread_work(void *dstPixels, unsigned int numThreads) {
    const std::chrono::steady_clock::time_point start(std::chrono::steady_clock::now());
    do {
        sampler_->renderFrame(static_cast<unsigned int *>(dstPixels), numThreads);
        FPS();
    } while (working_ != STOPPED);
    const std::chrono::steady_clock::time_point end(std::chrono::steady_clock::now());
    LOG ("TEMPO = %l ms",
         std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    if (working_ != STOPPED) {
        working_ = FINISH;
        LOG("%s", "WORKING = FINISH");
    }
    delete camera_;
    delete scene_;
    delete shader_;
    delete sampler_;
    delete rayTracer_;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap,
        jint nThreads) {
    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    AndroidBitmap_unlockPixels(env, dstBitmap);
    working_ = BUSY;
    LOG("%s", "WORKING = BUSY");
    thread_ = std::thread(thread_work, dstPixels, static_cast<unsigned int> (nThreads));
}

extern "C"
int Java_com_example_puscas_mobileraytracer_DrawView_redraw(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap) {
    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    AndroidBitmap_unlockPixels(env, dstBitmap);
    return working_;
}

extern "C"
int Java_com_example_puscas_mobileraytracer_DrawView_traceTouch(
        JNIEnv *,
        jobject,
        jfloat jx,
        jfloat jy) {
    const float x((float) jx / width_);
    const float y((float) jy / height_);
    Ray ray;
    Intersection intersection;
    const float u_alpha(fastArcTan(camera_->hFov_ * (x - 0.5f)));
    const float v_alpha(fastArcTan(camera_->vFov_ * (0.5f - y)));
    camera_->getRay(ray, u_alpha, v_alpha);
    const int primitiveID(rayTracer_->traceTouch(intersection, ray));
    return primitiveID;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_moveTouch(
        JNIEnv *,
        jobject,
        jfloat jx,
        jfloat jy,
        jint primitiveIndex
) {
    const float x((float) jx / width_);
    const float y((float) jy / height_);
    const float u_alpha(fastArcTan(camera_->hFov_ * (x - 0.5f)));
    const float v_alpha(fastArcTan(camera_->vFov_ * (0.5f - y)));
    const Material material;
    const Plane plane(Plane(Point3D(0.0f, 0.0f, scene_->primitives[primitiveIndex]->shape_->getZ()),
                            Vector3D(0.0f, 0.0f, -1.0f)));
    Intersection intersection;
    Ray ray;
    camera_->getRay(ray, u_alpha, v_alpha);
    plane.intersect(intersection, ray, material);
    scene_->primitives[primitiveIndex]->shape_->moveTo(intersection.point_.x_,
                                                       intersection.point_.y_);
    LOG("moveTouch (x,y)=(%f,%f)=(%f,%f)=(%f,%f)", jx, jy, x, y, u_alpha, v_alpha);
}

extern "C"
float Java_com_example_puscas_mobileraytracer_DrawView_getFPS(
        JNIEnv *,
        jobject
) {
    return fps_;
}