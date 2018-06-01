//
// Created by Tiago on 14-10-2016.
//

#include "DrawView.hpp"
#include <android/bitmap.h>
#include <glm/glm.hpp>
#include <mutex>

static ::State working_{State::IDLE};
static ::std::unique_ptr<::MobileRT::Renderer> renderer_{nullptr};
static ::std::unique_ptr<::JavaVM> javaVM_{nullptr};
static ::std::unique_ptr<::std::thread> thread_{nullptr};
static ::std::mutex mutex_{};
static ::std::int32_t width_{0};
static ::std::int32_t height_{0};
static float fps_{0.0f};
static ::std::int32_t timeFrame_{0};
static ::std::int32_t numberOfLights_{0};
static ::std::int32_t accelerator_{0};

extern "C"
::std::int32_t JNI_OnLoad(JavaVM *const jvm, void * /*reserved*/) {
    LOG("JNI_OnLoad");
    javaVM_.reset(jvm);

    JNIEnv *jniENV{nullptr};
    {
        const ::std::int32_t result {javaVM_->GetEnv(reinterpret_cast<void **>(&jniENV), JNI_VERSION_1_6)};
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
        const ::std::int32_t result {jniENV->CallStaticIntMethod(drawViewClass, drawViewMethodId)};
        assert(result == JNI_OK);
        static_cast<void> (result);
    }
    jniENV->ExceptionClear();
    return JNI_VERSION_1_6;
}

extern "C"
void JNI_OnUnload(JavaVM * /*vm*/, void * /*reserved*/) {
    LOG("JNI_OnUnload");
}

extern "C"
jobject Java_puscas_mobilertapp_DrawView_initCameraArray(
        JNIEnv *env,
        jobject /*thiz*/
) noexcept {
    ::MobileRT::Camera *const camera{renderer_->camera_.get()};
    const unsigned long arraySize{20};
    const jlong arrayBytes{static_cast<jlong> (arraySize) * static_cast<jlong> (sizeof(jfloat))};
    jobject directBuffer{nullptr};
    float *const floatBuffer{new float[arraySize]};

    if (floatBuffer != nullptr) {
        directBuffer = env->NewDirectByteBuffer(floatBuffer, arrayBytes);
        if (directBuffer != nullptr) {
            int i{0};

            floatBuffer[i++] = camera->position_.x;
            floatBuffer[i++] = camera->position_.y;
            floatBuffer[i++] = camera->position_.z;
            floatBuffer[i++] = 1.0f;

            floatBuffer[i++] = camera->direction_.x;
            floatBuffer[i++] = camera->direction_.y;
            floatBuffer[i++] = camera->direction_.z;
            floatBuffer[i++] = 1.0f;

            floatBuffer[i++] = camera->up_.x;
            floatBuffer[i++] = camera->up_.y;
            floatBuffer[i++] = camera->up_.z;
            floatBuffer[i++] = 1.0f;

            floatBuffer[i++] = camera->right_.x;
            floatBuffer[i++] = camera->right_.y;
            floatBuffer[i++] = camera->right_.z;
            floatBuffer[i++] = 1.0f;

            ::Components::Perspective *perspective{
                    dynamic_cast<::Components::Perspective *>(camera)};
            ::Components::Orthographic *orthographic{
                    dynamic_cast<::Components::Orthographic *>(camera)};
            if (perspective != nullptr) {
                const float hFov{perspective->getHFov()};
                const float vFov{perspective->getVFov()};
                floatBuffer[i++] = hFov;
                floatBuffer[i++] = vFov;
                floatBuffer[i++] = 0.0f;
                floatBuffer[i++] = 0.0f;
            }
            if (orthographic != nullptr) {
                const float sizeH{orthographic->getSizeH()};
                const float sizeV{orthographic->getSizeV()};
                floatBuffer[i++] = 0.0f;
                floatBuffer[i++] = 0.0f;
                floatBuffer[i++] = sizeH;
                floatBuffer[i++] = sizeV;
            }
        }
    }
    env->ExceptionClear();
    return directBuffer;
}

extern "C"
jobject Java_puscas_mobilertapp_DrawView_initVerticesArray(
        JNIEnv *env,
        jobject /*thiz*/
) noexcept {
    const ::std::vector<::MobileRT::Primitive<::MobileRT::Triangle>> &triangles{
            renderer_->shader_->scene_.triangles_};
    const unsigned long arraySize{triangles.size() * 3 * 4};
    const jlong arrayBytes{static_cast<jlong> (arraySize) * static_cast<jlong> (sizeof(jfloat))};
    jobject directBuffer{nullptr};
    if (arraySize > 0) {
        float *const floatBuffer{new float[arraySize]};
        if (floatBuffer != nullptr) {
            directBuffer = env->NewDirectByteBuffer(floatBuffer, arrayBytes);
            if (directBuffer != nullptr) {
                int i{0};
                for (const ::MobileRT::Primitive<::MobileRT::Triangle> &triangle : triangles) {
                    const ::glm::vec4 &pointA{triangle.shape_.pointA_, 1.0f};
                    const ::glm::vec4 &pointB{pointA.x + triangle.shape_.AB_.x,
                                              pointA.y + triangle.shape_.AB_.y,
                                              pointA.z + triangle.shape_.AB_.z, 1.0f};
                    const ::glm::vec4 &pointC{pointA.x + triangle.shape_.AC_.x,
                                              pointA.y + triangle.shape_.AC_.y,
                                              pointA.z + triangle.shape_.AC_.z, 1.0f};

                    floatBuffer[i++] = pointA.x;
                    floatBuffer[i++] = pointA.y;
                    floatBuffer[i++] = -pointA.z;
                    floatBuffer[i++] = pointA.w;

                    floatBuffer[i++] = pointB.x;
                    floatBuffer[i++] = pointB.y;
                    floatBuffer[i++] = -pointB.z;
                    floatBuffer[i++] = pointB.w;

                    floatBuffer[i++] = pointC.x;
                    floatBuffer[i++] = pointC.y;
                    floatBuffer[i++] = -pointC.z;
                    floatBuffer[i++] = pointC.w;
                }
            }
        }
    }
    env->ExceptionClear();
    return directBuffer;
}

extern "C"
jobject Java_puscas_mobilertapp_DrawView_initColorsArray(
        JNIEnv *env,
        jobject /*thiz*/
) noexcept {
    ::std::vector<::MobileRT::Primitive<::MobileRT::Triangle>> &triangles{
            renderer_->shader_->scene_.triangles_};
    const unsigned long arraySize{triangles.size() * 3 * 4};
    const jlong arrayBytes{static_cast<jlong> (arraySize) * static_cast<jlong> (sizeof(jfloat))};
    jobject directBuffer{nullptr};
    if (arraySize > 0) {
        float *const floatBuffer{new float[arraySize]};
        if (floatBuffer != nullptr) {
            directBuffer = env->NewDirectByteBuffer(floatBuffer, arrayBytes);
            if (directBuffer != nullptr) {
                int i{0};
                for (const ::MobileRT::Primitive<::MobileRT::Triangle> &triangle : triangles) {
                    const ::glm::vec3 &kD{triangle.material_.Kd_};
                    const ::glm::vec3 &kS{triangle.material_.Ks_};
                    const ::glm::vec3 &kT{triangle.material_.Kt_};
                    const ::glm::vec3 &lE{triangle.material_.Le_};
                    ::glm::vec3 color{kD};

                    color = ::glm::all(::glm::greaterThan(kS, color)) ? kS : color;
                    color = ::glm::all(::glm::greaterThan(kT, color)) ? kT : color;
                    color = ::glm::all(::glm::greaterThan(lE, color)) ? lE : color;

                    floatBuffer[i++] = color.r;
                    floatBuffer[i++] = color.g;
                    floatBuffer[i++] = color.b;
                    floatBuffer[i++] = 1.0f;

                    floatBuffer[i++] = color.r;
                    floatBuffer[i++] = color.g;
                    floatBuffer[i++] = color.b;
                    floatBuffer[i++] = 1.0f;

                    floatBuffer[i++] = color.r;
                    floatBuffer[i++] = color.g;
                    floatBuffer[i++] = color.b;
                    floatBuffer[i++] = 1.0f;
                }
            }
            switch (accelerator_) {
                case ::MobileRT::Shader::BVH:
                    triangles.clear();
                    ::std::vector<MobileRT::Primitive<MobileRT::Triangle>>{}.swap(triangles);
                    break;

                default:
                    break;
            }
        }
    }
    env->ExceptionClear();
    return directBuffer;
}

static ::std::string
readTextAsset(JNIEnv *const env, jobject assetManager, const char *const filename) {
    AAssetManager *const mgr {AAssetManager_fromJava(env, assetManager)};
    ::std::vector<char> buffer {};
    AAsset *const asset {AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING)};

    //holds size of searched file
    const off64_t length {AAsset_getLength64(asset)};
    //keeps track of remaining bytes to read
    off64_t remaining {AAsset_getRemainingLength64(asset)};
    const size_t Mb {1000 * 1024}; // 1Mb is maximum chunk size for compressed assets
    size_t currChunk {};
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
        //read data chunk
        if (AAsset_read(asset, chunk.data(), currChunk) > 0) {// returns less than 0 on error
            //and append it to our vector
            buffer.insert(buffer.end(), chunk.begin(), chunk.end());
            remaining = AAsset_getRemainingLength64(asset);
        }
    }
    AAsset_close(asset);
    const ::std::string &res {buffer.begin(), buffer.end()};
    env->ExceptionClear();
    return res;
}

static void FPS() noexcept {
    static ::std::int32_t frame{0};
    static ::std::chrono::steady_clock::time_point timebase_{};
    ++frame;
    const ::std::chrono::steady_clock::time_point time{::std::chrono::steady_clock::now()};
    if (::std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count() > 1000) {
        fps_ = (frame * 1000.0f) /
               (::std::chrono::duration_cast<::std::chrono::milliseconds>(time - timebase_).count());
        timebase_ = time;
        frame = 0;
    }
}

extern "C"
::std::int32_t Java_puscas_mobilertapp_ViewText_isWorking(
        JNIEnv *const env,
        jobject /*thiz*/
) noexcept {
    const ::std::int32_t res{static_cast<::std::int32_t> (working_)};
    env->ExceptionClear();
    return res;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *const env,
        jobject /*thiz*/
) noexcept {
    if (renderer_ != nullptr) {
        renderer_->stopRender();
    }
    working_ = State::STOPPED;
    LOG("WORKING = STOPPED");
    env->ExceptionClear();
}

extern "C"
::std::int32_t Java_puscas_mobilertapp_DrawView_initialize(
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
    accelerator_ = accelerator;
    LOG("INITIALIZE");


    ::std::int32_t res{
            [=]() noexcept -> ::std::int32_t {
        {
            ::std::lock_guard<::std::mutex> lock(mutex_);
            renderer_ = nullptr;
        }
        const float ratio {
            ::std::max(static_cast<float>(width_) / height_, static_cast<float>(height_) / width_)};
        const float hfovFactor{width_ > height_ ? ratio : 1.0f};
        const float vfovFactor{width_ < height_ ? ratio : 1.0f};
        ::MobileRT::Scene scene_{};
        ::std::unique_ptr<MobileRT::Sampler> samplerPixel{};
        ::std::unique_ptr<MobileRT::Shader> shader_{};
        ::std::unique_ptr<MobileRT::Camera> camera{};
        ::glm::vec3 maxDist{0, 0, 0};
        switch (scene) {
            case 0: {
                const float fovX{45.0f * hfovFactor};
                const float fovY{45.0f * vfovFactor};
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        fovX, fovY);
                scene_ = cornellBoxScene(::std::move(scene_));
                maxDist = ::glm::vec3 {1, 1, 1};
            }
                break;

            case 1: {
                const float sizeH{10.0f * hfovFactor};
                const float sizeV{10.0f * vfovFactor};
                /*camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {4.0f, 4.0f, -8.0f},
                        ::glm::vec3 {4.0f, 4.0f, 4.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);*/
                camera = ::std::make_unique<Components::Orthographic>(
                        ::glm::vec3 {0.0f, 1.0f, -10.0f},
                        ::glm::vec3 {0.0f, 1.0f, 7.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        sizeH, sizeV);
                /*camera = ::std::make_unique<Components::Perspective>(
                  ::glm::vec3 {0.0f, 0.5f, 1.0f},
                  ::glm::vec3 {0.0f, 0.0f, 7.0f},
                  ::glm::vec3 {0.0f, 1.0f, 0.0f},
                  60.0f  * hfovFactor, 60.0f * vfovFactor);*/
                scene_ = spheresScene(::std::move(scene_));
                maxDist = ::glm::vec3 {8, 8, 8};
            }
                break;

            case 2: {
                const float fovX{45.0f * hfovFactor};
                const float fovY{45.0f * vfovFactor};
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        fovX, fovY);
                scene_ = cornellBoxScene2(::std::move(scene_));
                maxDist = ::glm::vec3 {1, 1, 1};
            }
                break;

            case 3: {
                const float fovX{60.0f * hfovFactor};
                const float fovY{60.0f * vfovFactor};
                camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.5f, 1.0f},
                        ::glm::vec3 {0.0f, 0.0f, 7.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        fovX, fovY);
                scene_ = spheresScene2(::std::move(scene_));
                maxDist = ::glm::vec3 {8, 8, 8};
            }
                break;

            default: {
                jboolean isCopy {JNI_FALSE};
                const char *const objFileName {(env)->GetStringUTFChars(objFile, &isCopy)};
                const char *const matFileName {(env)->GetStringUTFChars(matFile, &isCopy)};
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

                const float fovX{45.0f * hfovFactor};
                const float fovY{45.0f * vfovFactor};
                //cornellbox
                /*camera = ::std::make_unique<Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.7f, 3.0f},
                        ::glm::vec3 {0.0f, 0.7f, -1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);*/
                const ::MobileRT::Material &lightMat{::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                     ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                     ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                     1.0f,
                                                     ::glm::vec3 {0.9f, 0.9f, 0.9f}};

                //conference
                /*scene_.lights_.emplace_back(::std::make_unique<::Components::PointLight>(
                        lightMat, ::glm::vec3 {0.0f, 1000.0f, 0.0f}));*/
                ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                    ::std::make_unique<Components::StaticHaltonSeq>()};
                scene_.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                        lightMat,
                        ::std::move(samplerPoint1),
                        ::glm::vec3 {-100.0f, 640.0f, -100.0f},
                        ::glm::vec3 {100.0f, 640.0f, -100.0f},
                        ::glm::vec3 {100.0f, 640.0f, 100.0f}));
                ::std::unique_ptr<MobileRT::Sampler> samplerPoint2 {
                    ::std::make_unique<Components::StaticHaltonSeq>()};
                scene_.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                        lightMat,
                        ::std::move(samplerPoint2),
                        ::glm::vec3 {-100.0f, 640.0f, -100.0f},
                        ::glm::vec3 {100.0f, 640.0f, 100.0f},
                        ::glm::vec3 {-100.0f, 640.0f, 100.0f}));
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {460.0f, 500.0f, -1000.0f},
                        ::glm::vec3 {0.0f, 400.0f, 0.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        fovX, fovY);

                //cornell spheres
                /*camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.7f, 3.0f},
                        ::glm::vec3 {0.0f, 0.7f, -1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        fovX, fovY);*/

                //teapot
                /*camera = ::std::make_unique<::Components::Perspective> (
                         ::glm::vec3 {0.0f, 30.0f, -200.0f},
                         ::glm::vec3 {0.0f, 30.0f, 100.0f},
                         ::glm::vec3 {0.0f, 1.0f, 0.0f},
                         fovX, fovY);*/
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
                                                                ::MobileRT::Shader::Accelerator(
                                                                        accelerator));
                break;
            }

            case 2: {
                ::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette{
                        ::std::make_unique<Components::StaticHaltonSeq>()};

                shader_ = ::std::make_unique<Components::PathTracer>(
                        ::std::move(scene_), ::std::move(samplerRussianRoulette), samplesLight,
                        ::MobileRT::Shader::Accelerator(accelerator));
                break;
            }

            case 3: {
                shader_ = ::std::make_unique<Components::DepthMap>(::std::move(scene_), maxDist,
                                                                 ::MobileRT::Shader::Accelerator(
                                                                         accelerator));
                break;
            }

            case 4: {
                shader_ = ::std::make_unique<Components::DiffuseMaterial>(::std::move(scene_),
                                                                        ::MobileRT::Shader::Accelerator(
                                                                                accelerator));
                break;
            }

            default: {
                shader_ = ::std::make_unique<Components::NoShadows>(::std::move(scene_), samplesLight,
                                                                  ::MobileRT::Shader::Accelerator(
                                                                          accelerator));
                break;
            }
        }
                const ::std::int32_t triangles{
                static_cast<int32_t> (shader_->scene_.triangles_.size())};
                const ::std::int32_t spheres{
                static_cast<int32_t> (shader_->scene_.spheres_.size())};
                const ::std::int32_t planes{
                        static_cast<::std::int32_t> (shader_->scene_.planes_.size())};
                numberOfLights_ = static_cast<::std::int32_t> (shader_->scene_.lights_.size());
                const ::std::int32_t nPrimitives{triangles + spheres + planes};
        {
            ::std::lock_guard<::std::mutex> lock(mutex_);
            renderer_ = ::std::make_unique<::MobileRT::Renderer>(
                    ::std::move(shader_), ::std::move(camera), ::std::move(samplerPixel),
                    static_cast<::std::uint32_t>(width_), static_cast<::std::uint32_t>(height_),
                    static_cast<::std::uint32_t>(samplesPixel));
        }
        /*LOG("TRIANGLES = ", triangles);
        LOG("SPHERES = ", spheres);
        LOG("PLANES = ", planes);
        LOG("LIGHTS = ", numberOfLights_);*/
        return nPrimitives;
    }()};


    //LOG("PRIMITIVES = ", res);
    env->ExceptionClear();
    return res;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_finishRender(
        JNIEnv *const env,
        jobject /*thiz*/
) noexcept {
    if (renderer_ != nullptr) {
        renderer_->stopRender();
    }
    if (thread_ != nullptr) {
        {
            ::std::lock_guard<::std::mutex> lock(mutex_);
            renderer_.reset();
            renderer_ = nullptr;
            thread_.reset();
            thread_ = nullptr;
            LOG("DELETED RENDERER");
            renderer_ = nullptr;
        }
    }
    working_ = State::IDLE;
    LOG("WORKING = IDLE");
    timeFrame_ = 0;
    fps_ = 0.0f;
    env->ExceptionClear();
}

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *const env,
        jobject /*thiz*/,
        jobject localBitmap,
        jint const nThreads,
        jboolean const async
) noexcept {
    jclass localDrawViewClass{env->FindClass("puscas/mobilertapp/DrawView")};
    jclass globalDrawViewClass{static_cast<jclass>(env->NewGlobalRef(localDrawViewClass))};
    jobject globalBitmap{static_cast<jobject>(env->NewGlobalRef(localBitmap))};

    jmethodID drawViewMethodId{
            env->GetStaticMethodID(globalDrawViewClass, "calledByJNI_static", "()I")};
    assert(drawViewMethodId != nullptr);
    {
        const ::std::int32_t result {env->CallStaticIntMethod(globalDrawViewClass, drawViewMethodId)};
        static_cast<void> (result);
    }

    drawViewMethodId = env->GetMethodID(globalDrawViewClass, "calledByJNI", "()I");
    assert(drawViewMethodId != nullptr);
    jobject resultObj = env->AllocObject(globalDrawViewClass);
    {
        const ::std::int32_t result {env->CallNonvirtualIntMethod(resultObj, globalDrawViewClass, drawViewMethodId)};
        assert(result == JNI_OK);
        static_cast<void> (result);
    }

    {
        const ::std::int32_t result {env->CallIntMethod(resultObj, drawViewMethodId)};
        assert(result == JNI_OK);
        static_cast<void> (result);
    }

    working_ = State::BUSY;
    LOG("WORKING = BUSY");

    auto lambda {
        [=]() noexcept -> void {
        assert(env != nullptr);
        const ::std::int32_t jniError {
                javaVM_->GetEnv(reinterpret_cast<void **>(const_cast<JNIEnv **>(&env)),
                                JNI_VERSION_1_6)};

        assert(jniError == JNI_OK || jniError == JNI_EDETACHED);
        {
            const ::std::int32_t result {javaVM_->AttachCurrentThread(const_cast<JNIEnv **>(&env), nullptr)};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }
        const ::std::int32_t jniThread {jniError == JNI_EDETACHED? JNI_EDETACHED : JNI_OK};

        assert(globalDrawViewClass != nullptr);
        jmethodID drawViewMethodId_thread{
                env->GetStaticMethodID(globalDrawViewClass, "calledByJNI_static", "()I")};
        assert(drawViewMethodId_thread != nullptr);
        const ::std::int32_t result_thread{
                env->CallStaticIntMethod(globalDrawViewClass, drawViewMethodId_thread)};
        LOG("result_thread = ", result_thread);

        ::std::uint32_t *dstPixels{nullptr};
        {
            const ::std::int32_t ret {AndroidBitmap_lockPixels(env, globalBitmap, reinterpret_cast<void **>(&dstPixels))};
            //dstPixels = static_cast<::std::uint32_t *>(env->GetDirectBufferAddress(globalByteBuffer));
            assert(ret == JNI_OK);
            LOG("ret = ", ret);
        }

        AndroidBitmapInfo info{};
        {
            const ::std::int32_t ret {AndroidBitmap_getInfo(env, globalBitmap, &info)};
            assert(ret == JNI_OK);
            LOG("ret = ", ret);
        }

        const ::std::uint32_t stride{info.stride};
        ::std::int32_t rep {1};
        do {
            const ::std::chrono::steady_clock::time_point start{
                    ::std::chrono::steady_clock::now()};
            LOG("STARTING RENDERING");
            LOG("nThreads = ", nThreads);
            {
                ::std::lock_guard<::std::mutex> lock(mutex_);
                if (renderer_ != nullptr) {
                    renderer_->renderFrame(dstPixels, nThreads, stride);
                }
            }
            LOG("FINISHED RENDERING");
            timeFrame_ = static_cast<int> (::std::chrono::duration_cast<::std::chrono::milliseconds>(
                    ::std::chrono::steady_clock::now() - start).count());
            FPS();
            rep--;
        } while (working_ != State::STOPPED && rep > 0);
        if (working_ != State::STOPPED) {
            working_ = State::FINISHED;
            LOG("WORKING = FINISHED");
        }
        {
            const ::std::int32_t result {AndroidBitmap_unlockPixels(env, globalBitmap)};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }

        env->DeleteGlobalRef(globalDrawViewClass);
        env->DeleteGlobalRef(globalBitmap);
        {
            const ::std::int32_t result {javaVM_->GetEnv(reinterpret_cast<void **>(const_cast<JNIEnv **>(&env)),
                                              JNI_VERSION_1_6)};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }
            env->ExceptionClear();
        if (jniThread == JNI_EDETACHED) {
            const ::std::int32_t result {javaVM_->DetachCurrentThread()};
            assert(result == JNI_OK);
            static_cast<void> (result);
        }
    }};

    if (async) {
        thread_ = ::std::make_unique<::std::thread>(lambda);
        thread_->detach();
    } else {
        lambda();
    }
    env->ExceptionClear();
}

extern "C"
::std::int32_t Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *const env,
        jobject /*thiz*/,
        jfloat const /*jx*/,
        jfloat const /*jy*/) noexcept {
    //const float u {static_cast<float> (jx) / width_};
    //const float v {static_cast<float> (jy) / height_};
    //const ::MobileRT::Ray &ray {renderer_->camera_->generateRay (u, v, 0.0f, 0.0f)};
    //::MobileRT::Intersection intersection {};
    //const ::std::int32_t primitiveID {renderer_->shader_->traceTouch(&intersection, ray)};
    const ::std::int32_t primitiveID {-1};
    env->ExceptionClear();
    return primitiveID;
}

extern "C"
void Java_puscas_mobilertapp_ViewText_moveTouch(
        JNIEnv *const env,
        jobject /*thiz*/,
        jfloat const /*jx*/,
        jfloat const /*jy*/,
        jint const /*primitiveIndex*/
) noexcept {
    /*const float u {static_cast<float> (jx) / width_};
    const float v {static_cast<float> (jy) / height_};
    const ::MobileRT::Ray &ray {renderer_->camera_->generateRay(u, v, 0.0f, 0.0f)};
    const ::std::uint32_t index {static_cast<::std::uint32_t>(primitiveIndex)};
    const ::MobileRT::Plane plane {
            ::glm::vec3 {0.0f, 0.0f, renderer_->shader_->scene_.planes_[index].shape_.getZ()},
            ::glm::vec3 {0.0f, 0.0f, -1.0f}};
    ::MobileRT::Intersection intersection {};
    plane.intersect(ray);
    renderer_->shader_->scene_.planes_[index].shape_.moveTo(intersection.point_[0],
                                                            intersection.point_[1];*/
    env->ExceptionClear();
}

extern "C"
float Java_puscas_mobilertapp_ViewText_getFPS(
        JNIEnv *const env,
        jobject /*thiz*/
) noexcept {
    env->ExceptionClear();
    return fps_;
}

extern "C"
::std::int32_t Java_puscas_mobilertapp_ViewText_getTimeFrame(
        JNIEnv *const env,
        jobject /*thiz*/
) noexcept {
    env->ExceptionClear();
    return timeFrame_;
}

extern "C"
::std::int32_t Java_puscas_mobilertapp_ViewText_getSample(
        JNIEnv *const env,
        jobject /*thiz*/
) noexcept {
    ::std::int32_t res{0};
    /*{
        ::std::lock_guard<::std::mutex> lock(mutex_);
        if (renderer_ != nullptr) {
            res = renderer_->getSample();
        }
    }*/
    env->ExceptionClear();
    return res;
}

extern "C"
::std::int32_t Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv *const env,
        jobject /*thiz*/,
        jint const size
) noexcept {
    const ::std::int32_t res{::MobileRT::roundDownToMultipleOf(size,
                                                               static_cast<::std::int32_t>(::std::sqrt(
                                                                       MobileRT::NumberOfBlocks)))};
    env->ExceptionClear();
    return res;
}

extern "C"
::std::int32_t Java_puscas_mobilertapp_DrawView_getNumberOfLights(
        JNIEnv *const env,
        jobject /*thiz*/
) noexcept {
    env->ExceptionClear();
    return numberOfLights_;
}

extern "C"
jobject Java_puscas_mobilertapp_DrawView_freeNativeBuffer(
        JNIEnv *env,
        jobject /*thiz*/,
        jobject bufferRef
) noexcept {
    if (bufferRef != nullptr) {
        void *buffer{env->GetDirectBufferAddress(bufferRef)};
        float *const floatBuffer{static_cast<float *>(buffer)};
        delete[] floatBuffer;
    }
    return nullptr;
}
