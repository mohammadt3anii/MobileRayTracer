//
// Created by Tiago on 14-10-2016.
//

#include "DrawView.h"

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

static int working_(IDLE);

static MobileRT::Scene *scene_(nullptr);
static MobileRT::Camera *camera_(nullptr);
static MobileRT::Shader *shader_(nullptr);
static MobileRT::Sampler *samplerCamera_(nullptr);
static MobileRT::Sampler *samplerPixel_(nullptr);
static MobileRT::Sampler *samplerRay_(nullptr);
static MobileRT::Sampler *samplerPointLight_(nullptr);
static MobileRT::Sampler *samplerLight_(nullptr);
static MobileRT::Renderer *renderer_(nullptr);
static std::thread *thread_(nullptr);
static unsigned int samplesPixel_(0u);
static unsigned int samplesLight_(0u);
static unsigned int numberOfBlocks_(4u);
static unsigned int blockSizeX_(0u);
static unsigned int blockSizeY_(0u);

static unsigned int width_(0u);
static unsigned int height_(0u);
static float fps_(0.0f);
static long long timeFrame_(0ll);

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

MobileRT::Scene *cornellBoxScene(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();
    // point light - white
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 0.99f, 0.0f)));

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.7f, 0.7f, 0.7f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                                           lightGrayMat));

    // front wall - light blue
    const MobileRT::Material lightBlueMat(MobileRT::RGB(0.0f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, -3.5f), MobileRT::Vector3D(0.0f, 0.0f, 1.0f)),
                                                           lightBlueMat));
    // floor - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                                           lightGrayMat));
    // ceiling - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                                           lightGrayMat));
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat));

    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    const MobileRT::Material GreenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.2f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 1.001f)), yellowMat));

    // triangle - green
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(-0.5f, 0.5f, 1.0f),
            MobileRT::Point3D(0.5f, 0.5f, 1.0f),
            MobileRT::Point3D(-0.5f, -0.5f, 1.0f)), greenMat));

    return &scene;
}

MobileRT::Scene *cornellBoxScene2(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();

    const unsigned long long int max(static_cast<unsigned long long int> (-1));
    LOG("samplesLight = %u, max = %llu", samplesLight_, max);
    const unsigned long long int domainPointLight(/*roundUpPower2*/(
                                                                           (width_ * height_ *
                                                                            2llu) * 2llu *
                                                                           samplesLight_ *
                                                                           samplesPixel_ *
                                                                           RAY_DEPTH_MAX));
    LOG("domainPointLight = %llu", domainPointLight);
    LOG("width_ = %u", width_);
    LOG("height_ = %u", height_);
    LOG("samplesLight_ = %u", samplesLight_);
    LOG("samplesPixel_ = %u", samplesPixel_);
    LOG("RAY_DEPTH_MAX = %u", RAY_DEPTH_MAX);
    //samplerPointLight_ = new Components::HaltonSeq(domainPointLight, 1u);
    samplerPointLight_ = new Components::Random(domainPointLight, 1u);

    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));

    scene.lights_.emplace_back(new Components::AreaLight(lightMat,
                                                         *samplerPointLight_,
                                                         MobileRT::Point3D(-0.25f, 0.99f, -0.25f),
                                                         MobileRT::Point3D(0.25f, 0.99f, -0.25f),
                                                         MobileRT::Point3D(0.25f, 0.99f, 0.25f)));

    scene.lights_.emplace_back(new Components::AreaLight(lightMat,
                                                         *samplerPointLight_,
                                                         MobileRT::Point3D(0.25f, 0.99f, 0.25f),
                                                         MobileRT::Point3D(-0.25f, 0.99f, 0.25f),
                                                         MobileRT::Point3D(-0.25f, 0.99f, -0.25f)));

    // block light - black
    /*const MobileRT::Material blockMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.3f, 0.98f, -0.3f),
            MobileRT::Point3D(-0.3f, 0.98f, -0.3f),
            MobileRT::Point3D(0.3f, 0.98f, 0.3f)), blockMat));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(-0.3f, 0.98f, 0.3f),
            MobileRT::Point3D(0.3f, 0.98f, 0.3f),
            MobileRT::Point3D(-0.3f, 0.98f, -0.3f)), blockMat));*/

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.7f, 0.7f, 0.7f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                                           lightGrayMat));

    // front wall - light blue
    const MobileRT::Material lightBlueMat(MobileRT::RGB(0.0f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, -4.0f), MobileRT::Vector3D(0.0f, 0.0f, 1.0f)),
                                                           lightBlueMat));

    // floor - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                                           lightGrayMat));
    // ceiling - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                                           lightGrayMat));
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat));

    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - transmission
    const MobileRT::Material TransmissionMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.9f, 0.9f, 0.9f), 1.1f);
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(-0.4f, -0.3f, 0.0f), 0.35f), TransmissionMat));

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 1.0f), MobileRT::Point3D(-0.5f, -0.5f, 1.0f),
            MobileRT::Point3D(0.5f, 0.5f, 1.0f)), yellowMat));

    // triangle - green
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(-0.5f, 0.5f, 1.0f),
            MobileRT::Point3D(0.5f, 0.5f, 1.0f),
            MobileRT::Point3D(-0.5f, -0.5f, 1.0f)), greenMat));

    return &scene;
}

MobileRT::Scene *spheresScene(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f),
                    mirrorMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                                        MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                    sandMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f),
                    greenMat));
    return &scene;
}

MobileRT::Scene *spheresScene2(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.4f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 2.0f, 5.0f), 0.3f), blueMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f), mirrorMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(1.5f, 0.5f, 5.0f), 0.2f), yellowMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                                        MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                    sandMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f), greenMat));
    return &scene;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv *,// env,
        jobject//this
) {
    thread_->join();
    delete thread_;

    delete samplerCamera_;
    delete samplerPointLight_;
    delete samplerRay_;
    delete camera_;
    delete scene_;
    delete shader_;
    delete renderer_;

    working_ = IDLE;
    LOG("%s", "WORKING = IDLE");
    timeFrame_ = 0ll;
    fps_ = 0.0f;
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
    //timeFrame_ = 0;
    //fps_ = 0.0f;
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
        jint samplesPixel,
        jint samplesLight
) {
    working_ = IDLE;
    LOG("%s", "WORKING = IDLE");
    width_ = static_cast<unsigned int>(width);
    height_ = static_cast<unsigned int>(height);
    const float ratio(static_cast<float>(height) / static_cast<float>(width));

    blockSizeX_ = width_ / numberOfBlocks_;
    blockSizeY_ = height_ / numberOfBlocks_;
    samplesPixel_ = static_cast<unsigned int>(samplesPixel);
    samplesLight_ = static_cast<unsigned int>(samplesLight);
    LOG("samplesPixel_ = %u", samplesPixel_);
    LOG("samplesLight_ = %u", samplesLight_);
    switch (scene) {
        case 1:
            camera_ = new Components::Orthographic(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                   MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                   MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                   6.5f, 4.5f);
            scene_ = spheresScene();
            break;

        case 2:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  60.0f, 60.0f * ratio);
            scene_ = spheresScene2();
            break;

        case 3:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene2();
            break;

        default:
            camera_ = new Components::Orthographic(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                   MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                   MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                   2.0f, 2.0f);
            scene_ = cornellBoxScene();
            break;
    }
    switch (sampler) {
        case 1:
            if (samplesPixel_ > 1u) {
                samplerPixel_ = new Components::HaltonSeq(width_ * height_ * 2llu * samplesPixel_,
                                                          1u);
            } else {
                samplerPixel_ = new Components::Constant(0.5f);
            }
            samplerCamera_ = new Components::HaltonSeq(width_, height_, samplesPixel_,
                                                       blockSizeX_, blockSizeY_);
            break;

        default:
            if (samplesPixel_ > 1u) {
                samplerPixel_ = new Components::Stratified(width_ * height_ * 2llu * samplesPixel_,
                                                           1u);
            } else {
                samplerPixel_ = new Components::Constant(0.5f);
            }
            samplerCamera_ = new Components::Stratified(width_, height_, samplesPixel_,
                                                        blockSizeX_, blockSizeY_);
            break;
    }
    samplerRay_ = nullptr;
    samplerPointLight_ = nullptr;
    const unsigned long long int domainRay(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX);
    const unsigned long long int domainLight(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX * samplesLight_);
    switch (shader) {
        case 1:
            shader_ = new Components::Whitted(*scene_, samplesLight_);
            break;

        case 2:
            LOG("domainRay = %llu, domainLight = %llu", domainRay, domainLight);
            //samplerRay_ = new Components::HaltonSeq(domainRay, 1u);
            samplerRay_ = new Components::Random(domainRay, 1u);
            //samplerLight_ = new Components::HaltonSeq(domainLight, 1);
            samplerLight_ = new Components::Random(domainLight, 1u);
            shader_ = new Components::PathTracer(
                    *scene_, *samplerRay_, *samplerLight_, samplesLight_);
            break;

        default:
            shader_ = new Components::NoShadows(*scene_, samplesLight_);
            break;
    }
    renderer_ = new MobileRT::Renderer(*samplerCamera_, *shader_, *camera_, width_,
                                       height_, blockSizeX_, blockSizeY_, *samplerPixel_);

    if (dynamic_cast<Components::PathTracer *>(shader_) != NULL) {
        renderer_->registerToneMapper([&](const float value) {
            return 1.0f - std::cos(std::sqrt(std::sqrt(value)));
        });
    }


    LOG("x = %d [%d]", blockSizeX_, width_);
    LOG("y = %d [%d]", blockSizeY_, height_);
}

void thread_work(void *dstPixels, unsigned int numThreads) {
    try {
        unsigned int rep(2u);
        do {
            const std::chrono::steady_clock::time_point start(std::chrono::steady_clock::now());
            renderer_->renderFrame(static_cast<unsigned int *>(dstPixels), numThreads);
            timeFrame_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - start).count();
            FPS();

            camera_->position_.x_ += 1.0f;
        } while (working_ != STOPPED && rep-- > 1u);
        if (working_ != STOPPED) {
            working_ = FINISHED;
            LOG("%s", "WORKING = FINISHED");
        }
    } catch (...) {
        LOG("%s", "EXCEPTION");
        raise(SIGTRAP);
    }
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
    const float u(static_cast<float> (jx) / width_);
    const float v(static_cast<float> (jy) / height_);
    const MobileRT::Ray ray(camera_->generateRay(u, v, 0.0f, 0.0f));
    MobileRT::Intersection intersection;
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
    const float u(static_cast<float> (jx) / width_);
    const float v(static_cast<float> (jy) / height_);
    const MobileRT::Ray ray(camera_->generateRay(u, v, 0.0f, 0.0f));
    const unsigned long index(static_cast<unsigned long>(primitiveIndex));
    const MobileRT::Material material;
    const MobileRT::Plane plane(
            MobileRT::Point3D(0.0f, 0.0f, scene_->primitives_[index]->shape_->getZ()),
            MobileRT::Vector3D(0.0f, 0.0f, -1.0f));
    MobileRT::Intersection intersection;
    plane.intersect(intersection, ray, material);
    scene_->primitives_[index]->shape_->moveTo(intersection.point_.x_, intersection.point_.y_);
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

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_getSample(
        JNIEnv *,
        jobject
) {
    return renderer_->getSample();
}

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv *,
        jobject,
        jint size
) {
    return roundDownToMultipleOf(static_cast<unsigned int> (size), numberOfBlocks_);
}
