//
// Created by Tiago on 14-10-2016.
//

#include "DrawView.hpp"

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

static int working_(IDLE);

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
static int64_t timeFrame_(0ll);

static void FPS() noexcept {
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

static MobileRT::Scene cornellBoxScene(MobileRT::Scene&& scene) noexcept {
    // point light - white
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
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

    return std::move(scene);
}

static MobileRT::Scene cornellBoxScene2(MobileRT::Scene&& scene) noexcept {
    const auto max(static_cast<uint64_t> (-1));
    LOG("samplesLight = ", samplesLight_, " max = ", max);
    const uint64_t domainPointLight(/*roundUpPower2*/
            ((width_ * height_ * 2ull) * 2ull * samplesLight_ * samplesPixel_ * RAY_DEPTH_MAX));
    LOG("domainPointLight = ", domainPointLight);
    LOG("width_ = ", width_);
    LOG("height_ = ", height_);
    LOG("samplesLight_ = ", samplesLight_);
    LOG("samplesPixel_ = ", samplesPixel_);
    LOG("RAY_DEPTH_MAX = ", RAY_DEPTH_MAX);
    //samplerPointLight_ = new Components::HaltonSeq(domainPointLight, 1);
    samplerPointLight_ = new Components::Random(domainPointLight, 1);

    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
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

    return std::move(scene);
}

static MobileRT::Scene spheresScene(MobileRT::Scene&& scene) noexcept {
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
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
    return std::move(scene);
}

static MobileRT::Scene spheresScene2(MobileRT::Scene&& scene) noexcept {
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
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
    return std::move(scene);
}

void thread_work(void *dstPixels, const unsigned int numThreads) noexcept {
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
        LOG("WORKING = FINISHED", "");
    }
}

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept {
    thread_->join();
    delete thread_;

    delete samplerCamera_;
    delete samplerPointLight_;
    delete samplerRay_;
    delete camera_;
    delete shader_;
    delete renderer_;

    working_ = IDLE;
    LOG("WORKING = IDLE", "");
    timeFrame_ = 0ll;
    fps_ = 0.0f;
}

extern "C"
int Java_puscas_mobilertapp_DrawView_isWorking(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept {
    return working_;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept {
    working_ = STOPPED;
    LOG("WORKING = STOPPED", "");
    renderer_->stopRender();
    //timeFrame_ = 0;
    //fps_ = 0.0f;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv * /*env*/,
        jobject /*thiz*/,
        jint const scene,
        jint const shader,
        jint const width,
        jint const height,
        jint const sampler,
        jint const samplesPixel,
        jint const samplesLight
) noexcept {
    working_ = IDLE;
    LOG("WORKING = IDLE", "");
    width_ = static_cast<unsigned int>(width);
    height_ = static_cast<unsigned int>(height);
    const float ratio(static_cast<float>(height) / static_cast<float>(width));

		MobileRT::Scene scene_;
    blockSizeX_ = width_ / numberOfBlocks_;
    blockSizeY_ = height_ / numberOfBlocks_;
    samplesPixel_ = static_cast<unsigned int>(samplesPixel);
    samplesLight_ = static_cast<unsigned int>(samplesLight);
    LOG("samplesPixel_ = ", samplesPixel_);
    LOG("samplesLight_ = ", samplesLight_);
    switch (scene) {
        case 1:
            camera_ = new Components::Orthographic(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                   MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                   MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                   6.5f, 4.5f);
            scene_ = spheresScene(std::move(scene_));
            break;

        case 2:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  60.0f, 60.0f * ratio);
            scene_ = spheresScene2(std::move(scene_));
            break;

        case 3:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene2(std::move(scene_));
            break;

        default:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene(std::move(scene_));
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
    const uint64_t domainRay(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX);
    const uint64_t domainLight(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX * samplesLight_);
    switch (shader) {
        case 1:
            shader_ = new Components::Whitted(std::move(scene_), samplesLight_);
            break;

        case 2: LOG("domainRay = ", domainRay, "domainLight = ", domainLight);
            //samplerRay_ = new Components::HaltonSeq(domainRay, 1u);
            samplerRay_ = new Components::Random(domainRay, 1u);
            //samplerLight_ = new Components::HaltonSeq(domainLight, 1);
            samplerLight_ = new Components::Random(domainLight, 1u);
            shader_ = new Components::PathTracer(
                    std::move(scene_), *samplerRay_, *samplerLight_, samplesLight_);
            break;

        default:
            shader_ = new Components::NoShadows(std::move(scene_), samplesLight_);
            break;
    }
    renderer_ = new MobileRT::Renderer(*samplerCamera_, *shader_, *camera_, width_,
                                       height_, blockSizeX_, blockSizeY_, *samplerPixel_);

    //Path Tracer needs tone mapper
    /*if (shader == 2) {
        renderer_->registerToneMapper([&](const float value) noexcept -> float {
            return 1.0f - std::cos(std::sqrt(std::sqrt(value)));
        });
    }*/


    LOG("x = ", blockSizeX_, "[", width_, "]");
    LOG("x = ", blockSizeY_, "[", height_, "]");
}

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *env,
        jobject /*thiz*/,
        jobject dstBitmap,
        jint const nThreads) noexcept {
    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    AndroidBitmap_unlockPixels(env, dstBitmap);
    working_ = BUSY;
    LOG("WORKING = BUSY", "");
    thread_ = new std::thread(thread_work, dstPixels, static_cast<unsigned int> (nThreads));
}

extern "C"
int Java_puscas_mobilertapp_DrawView_redraw(
        JNIEnv *env,
        jobject /*thiz*/,
        jobject dstBitmap) noexcept {
    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    AndroidBitmap_unlockPixels(env, dstBitmap);
    return working_;
}

extern "C"
int Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv * /*env*/,
        jobject /*thiz*/,
        jfloat const jx,
        jfloat const jy) noexcept {
    const float u(static_cast<float> (jx) / width_);
    const float v(static_cast<float> (jy) / height_);
    const MobileRT::Ray ray(camera_->generateRay(u, v, 0.0f, 0.0f));
    MobileRT::Intersection intersection;
    const int primitiveID(shader_->traceTouch(intersection, ray));
    return primitiveID;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_moveTouch(
        JNIEnv * /*env*/,
        jobject /*thiz*/,
        jfloat const jx,
        jfloat const jy,
        jint const primitiveIndex
) noexcept {
    const float u(static_cast<float> (jx) / width_);
    const float v(static_cast<float> (jy) / height_);
    const MobileRT::Ray ray(camera_->generateRay(u, v, 0.0f, 0.0f));
    const auto index(static_cast<uint32_t>(primitiveIndex));
    const MobileRT::Material material;
    const MobileRT::Plane plane(
            MobileRT::Point3D(0.0f, 0.0f, shader_->scene_.primitives_[index]->shape_.getZ()),
            MobileRT::Vector3D(0.0f, 0.0f, -1.0f));
    MobileRT::Intersection intersection;
    plane.intersect(intersection, ray, material);
    shader_->scene_.primitives_[index]->shape_.moveTo(intersection.point_.x_,
                                                      intersection.point_.y_);
}

extern "C"
float Java_puscas_mobilertapp_DrawView_getFPS(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept {
    return fps_;
}

extern "C"
int64_t Java_puscas_mobilertapp_DrawView_getTimeFrame(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept {
    return timeFrame_;
}

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_getSample(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept {
    return renderer_->getSample();
}

extern "C"
unsigned int Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv * /*env*/,
        jobject /*thiz*/,
        jint const size
) noexcept {
    return roundDownToMultipleOf(static_cast<unsigned int> (size), numberOfBlocks_);
}
