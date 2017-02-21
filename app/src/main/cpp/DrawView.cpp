//
// Created by Tiago on 14-10-2016.
//

#include "DrawView.h"

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

static int working_(IDLE);
static const MobileRT::Scene *scene_(nullptr);
static const MobileRT::Perspective *camera_(nullptr);
static MobileRT::Shader *shader_(nullptr);
static MobileRT::Sampler *samplerCamera_(nullptr);
static MobileRT::Sampler *samplerRay_(nullptr);
static MobileRT::Renderer *renderer_(nullptr);
static std::thread *thread_(nullptr);
static unsigned int width_(0);
static unsigned int height_(0);
static unsigned int blockSize_(1);
static float fps_(0.0f);
static long long timeFrame_(0);

void FPS(void) {
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

const MobileRT::Scene *cornellBoxScene(void) {
    MobileRT::Scene *const scene = new MobileRT::Scene();
    // point light - white
    scene->lights_.emplace_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                         MobileRT::Point3D(0.0f, 0.50f, 0.0f)));

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                                            lightGrayMat));
    // floor - white
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                                            lightGrayMat));
    // ceiling - white
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                                            lightGrayMat));
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat));
    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    const MobileRT::Material GreenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.2f, 0.0f));
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Sphere(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(1.0f, 1.0f, 0.0f));
    scene->primitives_.emplace_back(new MobileRT::Primitive(*new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 1.001f)), yellowMat));
    scene->cache();
    return scene;
}

const MobileRT::Scene *spheresScene(void) {
    MobileRT::Scene *const scene = new MobileRT::Scene();
    // create one light source
    scene->lights_.emplace_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                         MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene->primitives_.emplace_back(
            new MobileRT::Primitive(
                    *new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene->primitives_.emplace_back(
            new MobileRT::Primitive(
                    *new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f),
                    mirrorMat));
    scene->primitives_.emplace_back(
            new MobileRT::Primitive(*new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                                         MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                    sandMat));
    scene->primitives_.emplace_back(
            new MobileRT::Primitive(
                    *new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f),
                    greenMat));
    scene->cache();
    return scene;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv *,// env,
        jobject//this
) {
    thread_->join();
    delete thread_;
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
    renderer_->stopRender();
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
    width_ = roundToEvenNumber(width_);
    height_ = static_cast<unsigned int>(height);
    height_ = roundToEvenNumber(height_);

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
    blockSize_ = 8;
    switch (sampler) {
        case 1:
            samplerCamera_ = new MobileRT::HaltonSeq((width_ / blockSize_) * (height_ / blockSize_),
                                                     samples_);
            break;

        default:
            samplerCamera_ = new MobileRT::Stratified(
                    (width_ / blockSize_) * (height_ / blockSize_), samples_);
            break;
    }
    switch (shader) {
        case 1:
            shader_ = new MobileRT::Whitted(*scene_);
            break;

        case 2:
            samplerRay_ = new MobileRT::Stratified(width_ * height_, samples_);
            shader_ = new MobileRT::PathTracer(*scene_, *samplerRay_);
            break;

        default:
            shader_ = new MobileRT::NoShadows(*scene_);
            break;
    }
    renderer_ = new MobileRT::Renderer(*samplerCamera_, *shader_, *camera_, width_, height_,
                                       blockSize_);
}

void thread_work(void *dstPixels, unsigned int numThreads) {
    do {
        const std::chrono::steady_clock::time_point start(std::chrono::steady_clock::now());
        renderer_->renderFrame(static_cast<unsigned int *>(dstPixels), numThreads);
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
    delete samplerCamera_;
    delete samplerRay_;
    delete renderer_;
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
    thread_ = new std::thread(thread_work, dstPixels, static_cast<unsigned int> (nThreads));
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
    const float x(static_cast<float> (jx) / width_);
    const float y(static_cast<float> (jy) / height_);
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
    const float x(static_cast<float> (jx) / width_);
    const float y(static_cast<float> (jy) / height_);
    const float u_alpha(fastArcTan(camera_->hFov_ * (x - 0.5f)));
    const float v_alpha(fastArcTan(camera_->vFov_ * (0.5f - y)));
    const unsigned long index(static_cast<unsigned long>(primitiveIndex));
    const MobileRT::Material material;
    MobileRT::Plane plane(
            MobileRT::Point3D(0.0f, 0.0f, scene_->primitives_[index]->shape_->getZ()),
            MobileRT::Vector3D(0.0f, 0.0f, -1.0f));
    MobileRT::Ray ray_;
    camera_->getRay(ray_, u_alpha, v_alpha);
    MobileRT::Intersection intersection;
    plane.intersect(intersection, ray_, material);
    scene_->primitives_[index]->shape_->moveTo(intersection.point_.x_,
                                               intersection.point_.y_);
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
long long Java_puscas_mobilertapp_DrawView_getTimeFrame(
        JNIEnv *,
        jobject
) {
    return timeFrame_;
}
