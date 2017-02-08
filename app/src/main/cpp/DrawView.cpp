//
// Created by Tiago on 14-10-2016.
//

#include <android/bitmap.h>
#include <android/log.h>
#include <thread>

#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shaders/NoShadows.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shaders/Whitted.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Samplers/Stratified.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Samplers/Jittered.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shapes/Plane.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shapes/Sphere.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shapes/Triangle.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Cameras/Perspective.h"
#include "../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Lights/PointLight.h"

#define LOG(msg, ...)\
__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "line:%d: " msg, __LINE__, __VA_ARGS__);

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

static int working_(IDLE);
static const MobileRT::Scene *scene_(nullptr);
static const MobileRT::Perspective *camera_(nullptr);
static MobileRT::Sampler *sampler_(nullptr);
static MobileRT::Shader *shader_(nullptr);
static MobileRT::Intersection intersection_;
static MobileRT::Ray ray_;
static std::thread thread_;
static unsigned int width_(0);
static unsigned int height_(0);
static float fps_(0);
static long timeFrame_(0);

void FPS() {
    static int frame(0);
    static std::chrono::steady_clock::time_point timebase_;
    frame++;
    const std::chrono::steady_clock::time_point time(std::chrono::steady_clock::now());
    if (std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count() > 1000) {
        fps_ = (frame * 1000.0f) /
               (std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count());
        timebase_ = time;
        frame = 0;
    }
}

MobileRT::Scene *cornellBoxScene() {
    MobileRT::Scene *const scene = new MobileRT::Scene();
    // point light - white
    scene->lights_.push_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                      MobileRT::Point3D(0.0f, 0.50f, 0.0f)));

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                                         lightGrayMat));
    // floor - white
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                                         lightGrayMat));
    // ceiling - white
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                                         lightGrayMat));
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat));
    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    const MobileRT::Material GreenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.2f, 0.0f));
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Sphere(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(1.0f, 1.0f, 0.0f));
    scene->primitives_.push_back(new MobileRT::Primitive(*new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 1.001f)), yellowMat));
    return scene;
}

const MobileRT::Scene *spheresScene() {
    MobileRT::Scene *const scene = new MobileRT::Scene();
    // create one light source
    scene->lights_.push_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                      MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene->primitives_.push_back(
            new MobileRT::Primitive(
                    *new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene->primitives_.push_back(
            new MobileRT::Primitive(
                    *new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f),
                    mirrorMat));
    scene->primitives_.push_back(
            new MobileRT::Primitive(*new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                                         MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                    sandMat));
    scene->primitives_.push_back(
            new MobileRT::Primitive(
                    *new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f),
                    greenMat));
    return scene;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv *,// env,
        jobject//this
) {
    thread_.join();
    working_ = IDLE;
    LOG("%s", "WORKING = IDLE");
}

extern "C"
int Java_puscas_mobilertapp_DrawView_isWorking(
        JNIEnv *,// env,
        jobject//this
) {
    return working_;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *,// env,
        jobject//this
) {
    working_ = STOPPED;
    LOG("%s", "WORKING = STOPPED");
    sampler_->stopRender();
}

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
) {
    working_ = IDLE;
    LOG("%s", "WORKING = IDLE");
    const float ratio(static_cast<float>(height) / static_cast<float>(width));
    width_ = static_cast<unsigned int>(width);
    height_ = static_cast<unsigned int>(height);

    const unsigned int samples_(static_cast<unsigned int>(samples));
    switch (scene) {
        case 1:
            camera_ = new MobileRT::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f), 60.0f,
                                                60.0f * ratio);
            scene_ = spheresScene();
            break;

        default:
            camera_ = new MobileRT::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f), 45.0f,
                                                45.0f * ratio);
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
    switch (sampler) {
        case 1 :
            sampler_ = new MobileRT::Jittered(width_, height_, *shader_, samples_, *camera_);
            break;

        default:
            sampler_ = new MobileRT::Stratified(width_, height_, *shader_, samples_, *camera_);
            break;
    }
}

void thread_work(void *dstPixels, unsigned int numThreads) {
    do {
        const std::chrono::steady_clock::time_point start(std::chrono::steady_clock::now());
        sampler_->renderFrame(static_cast<unsigned int *>(dstPixels), numThreads);
        timeFrame_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count();
        FPS();
    } while (working_ != STOPPED);
    if (working_ != STOPPED) {
        working_ = FINISHED;
        LOG("%s", "WORKING = FINISHED");
    }
    delete camera_;
    delete scene_;
    delete shader_;
    delete sampler_;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
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
int Java_puscas_mobilertapp_DrawView_redraw(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap) {
    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    AndroidBitmap_unlockPixels(env, dstBitmap);
    return working_;
}

extern "C"
int Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *,
        jobject,
        jfloat jx,
        jfloat jy) {
    const float x((float) jx / width_);
    const float y((float) jy / height_);
    const float u_alpha(fastArcTan(camera_->hFov_ * (x - 0.5f)));
    const float v_alpha(fastArcTan(camera_->vFov_ * (0.5f - y)));
    MobileRT::Ray ray;
    MobileRT::Intersection intersection;
    camera_->getRay(ray, u_alpha, v_alpha);
    const int primitiveID(shader_->traceTouch(intersection, ray));
    return primitiveID;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_moveTouch(
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
    const MobileRT::Material material;
    const MobileRT::Plane plane(MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, scene_->primitives_[primitiveIndex]->shape_->getZ()),
            MobileRT::Vector3D(0.0f, 0.0f, -1.0f)));
    camera_->getRay(ray_, u_alpha, v_alpha);
    plane.intersect(intersection_, ray_, material);
    scene_->primitives_[primitiveIndex]->shape_->moveTo(intersection_.point_.x_,
                                                        intersection_.point_.y_);
    //LOG("moveTouch (x,y)=(%f,%f)=(%f,%f)=(%f,%f)", jx, jy, x, y, u_alpha, v_alpha);
}

extern "C"
float Java_puscas_mobilertapp_DrawView_getFPS(
        JNIEnv *,
        jobject
) {
    return fps_;
}

extern "C"
long Java_puscas_mobilertapp_DrawView_getTimeFrame(
        JNIEnv *,
        jobject
) {
    return timeFrame_;
}