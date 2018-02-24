//
// Created by Tiago on 14-10-2016.
//

#include "DrawView.hpp"
#include <android/bitmap.h>
#include <mutex>

static ::State working_{State::IDLE};
static ::MobileRT::Renderer *renderer_{nullptr};
static ::JavaVM *javaVM_{nullptr};
static ::std::thread *thread_{nullptr};
static ::std::mutex mutex_{};
static int width_{0};
static int height_{0};
static float fps_{0.0f};
static long long timeFrame_{0};
static long long numberOfLights_{0};

extern "C"
int JNI_OnLoad(JavaVM *const jvm, void * /*reserved*/) {
    LOG("JNI_OnLoad");
    javaVM_ = jvm;

    JNIEnv *jniENV{nullptr};
    {
        const int result {javaVM_->GetEnv(reinterpret_cast<void **>(&jniENV), JNI_VERSION_1_6)};
        assert(result == JNI_OK);
        static_cast<void> (result);
    }
    assert(jniENV != nullptr);
    jclass drawViewClass{jniENV->FindClass("puscas/mobilertapp/DrawView")};
    assert(drawViewClass != nullptr);
    jmethodID drawViewMethodId{
            jniENV->GetStaticMethodID(drawViewClass, "calledByJNI_static", "()I")};
    assert(drawViewMethodId != nullptr);
    {
        const int result {jniENV->CallStaticIntMethod(drawViewClass, drawViewMethodId)};
        assert(result == JNI_OK);
        static_cast<void> (result);
    }

    return JNI_VERSION_1_6;
}

extern "C"
void JNI_OnUnload(JavaVM * /*vm*/, void * /*reserved*/) {
    LOG("JNI_OnUnload");
}

static ::std::string
readTextAsset(JNIEnv *const env, jobject assetManager, const char *const filename) {
    AAssetManager *const mgr{AAssetManager_fromJava(env, assetManager)};
    ::std::vector<char> buffer{};
    AAsset *const asset{AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING)};

    //holds size of searched file
    const off64_t length{AAsset_getLength64(asset)};
    //keeps track of remaining bytes to read
    off64_t remaining{AAsset_getRemainingLength64(asset)};
    const size_t Mb{1000 * 1024}; // 1Mb is maximum chunk size for compressed assets
    size_t currChunk{};
    buffer.reserve(static_cast<size_t>(length));

    //while we have still some data to read
    while (remaining != 0) {
        //set proper size for our next chunk
        if (remaining >= static_cast<off64_t>(Mb)) {
            currChunk = Mb;
        } else {
            currChunk = static_cast<size_t>(remaining);
        }
        ::std::vector<char> chunk(currChunk);
        chunk.reserve(currChunk);
        //read data chunk
        if (AAsset_read(asset, chunk.data(), currChunk) > 0) {// returns less than 0 on error
            //and append it to our vector
            buffer.insert(buffer.end(), chunk.begin(), chunk.end());
            remaining = AAsset_getRemainingLength64(asset);
        }
    }
    AAsset_close(asset);
    const ::std::string res{buffer.begin(), buffer.end()};
    return res;
}

static void FPS() noexcept {
    static int frame{0};
    static ::std::chrono::steady_clock::time_point timebase_{};
    frame++;
    const ::std::chrono::steady_clock::time_point time{::std::chrono::steady_clock::now()};
    if (::std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count() > 1000) {
        fps_ = (frame * 1000.0f) /
               (::std::chrono::duration_cast<::std::chrono::milliseconds>(time - timebase_).count());
        timebase_ = time;
        frame = 0;
    }
}

extern "C"
State Java_puscas_mobilertapp_ViewText_isWorking(
        JNIEnv *const /*env*/,
        jobject /*thiz*/
) noexcept {
    return working_;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *const /*env*/,
        jobject /*thiz*/
) noexcept {
    renderer_->stopRender();
    working_ = State::STOPPED;
    LOG("WORKING = STOPPED");
}

extern "C"
long long Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv *const env,
        jobject /*thiz*/,
        jint const scene,
        jint const shader,
        jint const width,
        jint const height,
        jint const accelerator,
        jint const samplesPixel,
        jint const samplesLight,
        jstring objFile,
        jstring matFile,
        jobject assetManager
) noexcept {
    width_ = width;
    height_ = height;
    LOG("INITIALIZE");


    long long res = [=]() -> long long {
        mutex_.lock();
        renderer_ = nullptr;
        mutex_.unlock();
        const float ratio {
            ::std::max(static_cast<float>(width_) / height_, static_cast<float>(height_) / width_)};
        const float hfovFactor{width_ > height_ ? ratio : 1.0f};
        const float vfovFactor{width_ < height_ ? ratio : 1.0f};
        MobileRT::Scene scene_{};
        ::std::unique_ptr<MobileRT::Sampler> samplerPixel{};
        ::std::unique_ptr<MobileRT::Shader> shader_{};
        ::std::unique_ptr<MobileRT::Camera> camera{};
        ::glm::vec3 maxDist{0, 0, 0};
        switch (scene) {
            case 0:
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3{0.0f, 0.0f, -3.4f},
                        ::glm::vec3{0.0f, 0.0f, 1.0f},
                        ::glm::vec3{0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                scene_ = cornellBoxScene(::std::move(scene_));
                maxDist = ::glm::vec3 {1, 1, 1};
                break;

            case 1:
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3{4.0f, 4.0f, -8.0f},
                        ::glm::vec3{4.0f, 4.0f, 4.0f},
                        ::glm::vec3{0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                /*camera = ::std::make_unique<Components::Orthographic> (
                  ::glm::vec3{0.0f, 1.0f, - 10.0f},
                  ::glm::vec3{0.0f, 1.0f, 7.0f},
                  ::glm::vec3{0.0f, 1.0f, 0.0f},
                  10.0f * hfovFactor, 10.0f * vfovFactor);*/
                /*camera = ::std::make_unique<Components::Perspective>(
                  ::glm::vec3(0.0f, 0.5f, 1.0f),
                  ::glm::vec3(0.0f, 0.0f, 7.0f),
                  ::glm::vec3(0.0f, 1.0f, 0.0f),
                  60.0f * hfovFactor, 60.0f * vfovFactor);*/
                scene_ = spheresScene(::std::move(scene_));
                maxDist = ::glm::vec3 {8, 8, 8};
                break;

            case 2:
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                scene_ = cornellBoxScene2(::std::move(scene_));
                maxDist = ::glm::vec3 {1, 1, 1};
                break;

            case 3:
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.5f, 1.0f},
                        ::glm::vec3 {0.0f, 0.0f, 7.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        60.0f * hfovFactor, 60.0f * vfovFactor);
                scene_ = spheresScene2(::std::move(scene_));
                maxDist = ::glm::vec3 {8, 8, 8};
                break;

            default: {
                jboolean isCopy {JNI_FALSE};
                const char *objFileName {(env)->GetStringUTFChars(objFile, &isCopy)};
                const char *matFileName {(env)->GetStringUTFChars(matFile, &isCopy)};
                ::std::string obj {readTextAsset(env, assetManager, objFileName)};
                ::std::string mat {readTextAsset(env, assetManager, matFileName)};
                ::Components::OBJLoader objLoader {::std::move(obj), ::std::move(mat)};
                env->ReleaseStringUTFChars(objFile, objFileName);
                env->ReleaseStringUTFChars(matFile, matFileName);
                objLoader.process();
                if (!objLoader.isProcessed()) {
                    exit(0);
                }
                objLoader.fillScene(&scene_,
                                    []() { return ::std::make_unique<Components::StaticHaltonSeq>(); });
                //cornellbox
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.7f, 3.0f},
                        ::glm::vec3 {0.0f, 0.7f, -1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                const MobileRT::Material lightMat{::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                  ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                  ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                  1.0f,
                                                  ::glm::vec3 {0.9f, 0.9f, 0.9f}};
                scene_.lights_.emplace_back(::std::make_unique<Components::PointLight>(
                        lightMat, ::glm::vec3 {0.0f, 0.9f, 0.0f}));
                //teapot
                /*camera = ::std::make_unique<Components::Perspective> (
                  ::glm::vec3 {0.0f, 30.0f, -200.0f}, ::glm::vec3 {0.0f, 30.0f, 100.0f},
                  ::glm::vec3 {0.0f, 1.0f, 0.0f}, 45.0f * hfovFactor, 45.0f * vfovFactor);
                const MobileRT::Material lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                   ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                   ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                   1.0f,
                                                   ::glm::vec3 {0.9f, 0.9f, 0.9f}};
                scene_.lights_.emplace_back (::std::make_unique<Components::PointLight> (
                  lightMat, ::glm::vec3 {0.0f, 900.0f, 0.0f}));*/
                //conference
                /*camera = ::std::make_unique<Components::Perspective> (
                  ::glm::vec3 {-730.0f, 600.0f, -950.0f},
                  ::glm::vec3 {-400.0f, 300.0f, 0.0f},
                  ::glm::vec3 {0.0f, 1.0f, 0.0f}, 45.0f * hfovFactor, 45.0f * vfovFactor);
                const MobileRT::Material lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                   ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                   ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                   1.0f,
                                                   ::glm::vec3 {0.9f, 0.9f, 0.9f}};
                ::std::unique_ptr<MobileRT::Sampler> samplerPoint1 {
                  ::std::make_unique<Components::StaticHaltonSeq> ()};
                ::std::unique_ptr<MobileRT::Sampler> samplerPoint2 {
                  ::std::make_unique<Components::StaticHaltonSeq> ()};
                scene_.lights_.emplace_back (::std::make_unique<Components::AreaLight> (lightMat,
                                                                        ::std::move (samplerPoint1),
                                                                        ::glm::vec3 {-400.0f, 500.0f,
                                                                                           0.0f},
                                                                        ::glm::vec3 {-500.0f, 500.0f,
                                                                                           0.0f},
                                                                        ::glm::vec3 {-500.0f, 500.0f,
                                                                                           -100.0f}));
                scene_.lights_.emplace_back (::std::make_unique<Components::AreaLight> (lightMat,
                                                                        ::std::move (samplerPoint2),
                                                                        ::glm::vec3 {-500.0f, 500.0f,
                                                                                           -100.0f},
                                                                        ::glm::vec3 {-400.0f, 500.0f,
                                                                                           -100.0f},
                                                                        ::glm::vec3 {-400.0f, 500.0f,
                                                                                           0.0f}));*/
                /*scene_.lights_.emplace_back (::std::make_unique<Components::PointLight> (
                  lightMat, ::glm::vec3 {400.0f, 500.0f, -500.0f}));*/
                maxDist = ::glm::vec3 {1, 1, 1};
            }
                break;
        }
        if (samplesPixel > 1) {
            samplerPixel = ::std::make_unique<Components::StaticHaltonSeq>();
        } else {
            samplerPixel = ::std::make_unique<Components::Constant>(0.5f);
        }
        switch (shader) {
            case 1: {
                shader_ = ::std::make_unique<Components::Whitted>(::std::move(scene_), samplesLight,
                                                                MobileRT::Shader::Accelerator(
                                                                        accelerator));
                break;
            }

            case 2: {
                ::std::unique_ptr<MobileRT::Sampler> samplerRay{
                        ::std::make_unique<Components::StaticHaltonSeq>()};
                ::std::unique_ptr<MobileRT::Sampler> samplerLight{
                        ::std::make_unique<Components::StaticHaltonSeq>()};
                ::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette{
                        ::std::make_unique<Components::StaticHaltonSeq>()};

                shader_ = ::std::make_unique<Components::PathTracer>(
                        ::std::move(scene_), ::std::move(samplerRay), ::std::move(samplerLight),
                        ::std::move(samplerRussianRoulette), samplesLight,
                        MobileRT::Shader::Accelerator(accelerator));
                break;
            }

            case 3: {
                shader_ = ::std::make_unique<Components::DepthMap>(::std::move(scene_), maxDist,
                                                                 MobileRT::Shader::Accelerator(
                                                                         accelerator));
                break;
            }

            case 4: {
                shader_ = ::std::make_unique<Components::DiffuseMaterial>(::std::move(scene_),
                                                                        MobileRT::Shader::Accelerator(
                                                                                accelerator));
                break;
            }

            default: {
                shader_ = ::std::make_unique<Components::NoShadows>(::std::move(scene_), samplesLight,
                                                                  MobileRT::Shader::Accelerator(
                                                                          accelerator));
                break;
            }
        }
        mutex_.lock();
        renderer_ = new MobileRT::Renderer{
                ::std::move(shader_), ::std::move(camera), ::std::move(samplerPixel),
                static_cast<unsigned>(width_), static_cast<unsigned>(height_),
                static_cast<unsigned>(samplesPixel)};
        mutex_.unlock();
        const long long triangles{
                static_cast<long long> (renderer_->shader_->scene_.triangles_.size())};
        const long long spheres{
                static_cast<long long> (renderer_->shader_->scene_.spheres_.size())};
        const long long planes{static_cast<long long> (renderer_->shader_->scene_.planes_.size())};
        const long long rectangles{
                static_cast<long long> (renderer_->shader_->scene_.rectangles_.size())};
        numberOfLights_ = static_cast<long long> (renderer_->shader_->scene_.lights_.size());
        const long long nPrimitives = triangles + spheres + planes + rectangles;
        /*LOG("TRIANGLES = ", triangles);
        LOG("SPHERES = ", spheres);
        LOG("PLANES = ", planes);
        LOG("RECTANGLES = ", rectangles);
        LOG("LIGHTS = ", numberOfLights_);*/
        return nPrimitives;
    }();


    //LOG("PRIMITIVES = ", res);
    return res;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_finishRender(
        JNIEnv *const /*env*/,
        jobject /*thiz*/
) noexcept {
    if (thread_ != nullptr) {
        thread_->join();
        thread_ = nullptr;
        mutex_.lock();
        delete renderer_;
        LOG("DELETED RENDERER");
        renderer_ = nullptr;
        mutex_.unlock();
    }
    working_ = State::IDLE;
    LOG("WORKING = IDLE");
    timeFrame_ = 0;
    fps_ = 0.0f;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *const env,
        jobject /*thiz*/,
        jobject localBitmap,
        jint const nThreads
) noexcept {
    jclass localDrawViewClass{env->FindClass("puscas/mobilertapp/DrawView")};
    jclass globalDrawViewClass{static_cast<jclass>(env->NewGlobalRef(localDrawViewClass))};
    jobject globalBitmap{static_cast<jobject>(env->NewGlobalRef(localBitmap))};

    jmethodID drawViewMethodId{
            env->GetStaticMethodID(globalDrawViewClass, "calledByJNI_static", "()I")};
    assert(drawViewMethodId != nullptr);
    {
        const int result {env->CallStaticIntMethod(globalDrawViewClass, drawViewMethodId)};
        static_cast<void> (result);
    }

    drawViewMethodId = env->GetMethodID(globalDrawViewClass, "calledByJNI", "()I");
    assert(drawViewMethodId != nullptr);
    jobject resultObj = env->AllocObject(globalDrawViewClass);
    {
        const int result {env->CallNonvirtualIntMethod(resultObj, globalDrawViewClass, drawViewMethodId)};
        assert(result == JNI_OK);
        static_cast<void> (result);
    }

    {
        const int result {env->CallIntMethod(resultObj, drawViewMethodId)};
        assert(result == JNI_OK);
        static_cast<void> (result);
    }

    working_ = State::BUSY;
    LOG("WORKING = BUSY");

    auto lambda = [=]() {
        assert(env != nullptr);
        const int jniError {
                javaVM_->GetEnv(reinterpret_cast<void **>(const_cast<JNIEnv **>(&env)),
                                JNI_VERSION_1_6)};

        assert(jniError == JNI_OK || jniError == JNI_EDETACHED);
        {
            const int result {javaVM_->AttachCurrentThread(const_cast<JNIEnv **>(&env), nullptr)};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }
        const int jniThread {jniError == JNI_EDETACHED? JNI_EDETACHED : JNI_OK};

        assert(globalDrawViewClass != nullptr);
        jmethodID drawViewMethodId_thread{
                env->GetStaticMethodID(globalDrawViewClass, "calledByJNI_static", "()I")};
        assert(drawViewMethodId_thread != nullptr);
        const int result_thread{
                env->CallStaticIntMethod(globalDrawViewClass, drawViewMethodId_thread)};
        LOG("result_thread = ", result_thread);

        unsigned *dstPixels{nullptr};
        {
            const int ret {AndroidBitmap_lockPixels(env, globalBitmap, reinterpret_cast<void **>(&dstPixels))};
            //dstPixels = static_cast<unsigned *>(env->GetDirectBufferAddress(globalByteBuffer));
            assert(ret == JNI_OK);
            LOG("ret = ", ret);
        }

        AndroidBitmapInfo info{};
        {
            const int ret {AndroidBitmap_getInfo(env, globalBitmap, &info)};
            assert(ret == JNI_OK);
            LOG("ret = ", ret);
        }

        const unsigned stride{info.stride};
        int rep {1};
        do {
            const ::std::chrono::steady_clock::time_point start{
                    ::std::chrono::steady_clock::now()};
            LOG("STARTING RENDERING");
            LOG("nThreads = ", nThreads);
            mutex_.lock();
            if (renderer_ != nullptr) {
                renderer_->renderFrame(dstPixels, nThreads, stride);
            }
            mutex_.unlock();
            LOG("FINISHED RENDERING");
            timeFrame_ = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
                    ::std::chrono::steady_clock::now() - start).count();
            FPS();
            //renderer_->camera_->position_.x_ += 1.0f;
            rep--;
        } while (working_ != State::STOPPED && rep > 0);
        if (working_ != State::STOPPED) {
            working_ = State::FINISHED;
            LOG("WORKING = FINISHED");
        }
        {
            const int result {AndroidBitmap_unlockPixels(env, globalBitmap)};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }

        env->DeleteGlobalRef(globalDrawViewClass);
        env->DeleteGlobalRef(globalBitmap);
        {
            const int result {javaVM_->GetEnv(reinterpret_cast<void **>(const_cast<JNIEnv **>(&env)),
                                              JNI_VERSION_1_6)};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }
        if (jniThread == JNI_EDETACHED) {
            const int result {javaVM_->DetachCurrentThread()};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }
    };

    thread_ = new ::std::thread {lambda};
    //lambda();
}

extern "C"
int Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *const /*env*/,
        jobject /*thiz*/,
        jfloat const /*jx*/,
        jfloat const /*jy*/) noexcept {
    //const float u {static_cast<float> (jx) / width_};
    //const float v {static_cast<float> (jy) / height_};
    //MobileRT::Ray ray {renderer_->camera_->generateRay (u, v, 0.0f, 0.0f)};
    //MobileRT::Intersection intersection {};
    //const int primitiveID {renderer_->shader_->traceTouch(&intersection, ::std::move(ray))};
    const int primitiveID{-1};
    return primitiveID;
}

extern "C"
void Java_puscas_mobilertapp_ViewText_moveTouch(
        JNIEnv *const /*env*/,
        jobject /*thiz*/,
        jfloat const /*jx*/,
        jfloat const /*jy*/,
        jint const /*primitiveIndex*/
) noexcept {
    /*const float u{static_cast<float> (jx) / width_};
    const float v{static_cast<float> (jy) / height_};
    const MobileRT::Ray ray{renderer_->camera_->generateRay(u, v, 0.0f, 0.0f)};
    const uint32_t index{static_cast<uint32_t>(primitiveIndex)};
    const MobileRT::Plane plane{
            ::glm::vec3{0.0f, 0.0f, renderer_->shader_->scene_.planes_[index].shape_.getZ()},
            ::glm::vec3 {0.0f, 0.0f, -1.0f}};
    MobileRT::Intersection intersection{};
    plane.intersect(ray);
    renderer_->shader_->scene_.planes_[index].shape_.moveTo(intersection.point_.x_(),
                                                            intersection.point_.y_());*/
}

extern "C"
float Java_puscas_mobilertapp_ViewText_getFPS(
        JNIEnv *const /*env*/,
        jobject /*thiz*/
) noexcept {
    return fps_;
}

extern "C"
long long Java_puscas_mobilertapp_ViewText_getTimeFrame(
        JNIEnv *const /*env*/,
        jobject /*thiz*/
) noexcept {
    return timeFrame_;
}

extern "C"
unsigned Java_puscas_mobilertapp_ViewText_getSample(
        JNIEnv *const /*env*/,
        jobject /*thiz*/
) noexcept {
    unsigned res{0};
    /*mutex_.lock();
    if (renderer_ != nullptr) {
        res = renderer_->getSample();
    }
    mutex_.unlock();*/
    return res;
}

extern "C"
int Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv *const /*env*/,
        jobject /*thiz*/,
        jint const size
) noexcept {
    return MobileRT::roundDownToMultipleOf(size,
                                           static_cast<int>(::std::sqrt(MobileRT::NumberOfBlocks)));
}

extern "C"
long long Java_puscas_mobilertapp_DrawView_getNumberOfLights(
        JNIEnv *const /*env*/,
        jobject /*thiz*/
) noexcept {
    return numberOfLights_;
}
