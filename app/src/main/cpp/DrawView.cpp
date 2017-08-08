//
// Created by Tiago on 14-10-2016.
//

#include "DrawView.hpp"

static State working_ {State::IDLE};
static MobileRT::Renderer *renderer_ {nullptr};
static std::thread *thread_ {nullptr};
static int numberOfBlocks_ {4};
static int width_ {0};
static int height_ {0};
static float fps_ {0.0f};
static int64_t timeFrame_ {0};

static void FPS() noexcept {

  static int frame {0};
  static std::chrono::steady_clock::time_point timebase_ {};
    frame++;
  const std::chrono::steady_clock::time_point time {std::chrono::steady_clock::now ()};
    if (std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count() > 1000) {
        fps_ = (frame * 1000.0f) /
               (std::chrono::duration_cast<std::chrono::milliseconds>(time - timebase_).count());
        timebase_ = time;
        frame = 0;
    }
}

void thread_work(void *dstPixels, const int numThreads) noexcept {

  int rep {1};
    do {
      const std::chrono::steady_clock::time_point start {std::chrono::steady_clock::now ()};
        renderer_->renderFrame(static_cast<unsigned int *>(dstPixels), numThreads);
        timeFrame_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count();
        FPS();

        renderer_->camera_->position_.x_ += 1.0f;
    } while (working_ != State::STOPPED && rep -- > 1);
  if (working_ != State::STOPPED) {
    working_ = State::FINISHED;
        LOG("WORKING = FINISHED");
    }
}

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept {
    thread_->join();
    delete thread_;
    delete renderer_;

  working_ = State::IDLE;
    LOG("WORKING = IDLE");
  timeFrame_ = 0;
    fps_ = 0.0f;
}

extern "C"
State Java_puscas_mobilertapp_DrawView_isWorking (
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

  working_ = State::STOPPED;
    LOG("WORKING = STOPPED");
    renderer_->stopRender();
    //timeFrame_ = 0;
    //fps_ = 0.0f;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv * env,
        jobject /*thiz*/,
        jint const scene,
        jint const shader,
        jint const width,
        jint const height,
        jint const sampler,
        jint const samplesPixel,
        jint const samplesLight,
        jstring objFile,
        jstring matText
) noexcept {
    jboolean isCopy (JNI_FALSE);
  const char *obj {(env) -> GetStringUTFChars (objFile, &isCopy)};
  const char *mat {(env) -> GetStringUTFChars (matText, &isCopy)};
  Components::OBJLoader objLoader {obj, mat};
    env->ReleaseStringUTFChars(objFile, obj);
    env->ReleaseStringUTFChars(matText, mat);
    objLoader.process();
  if (! objLoader . isProcessed ()) {
    exit (0);
  }

  working_ = State::IDLE;
    LOG("WORKING = IDLE");
    width_ = width;
    height_ = height;
  const float ratio {static_cast<float>(height) / static_cast<float>(width)};
    renderer_ = nullptr;

  int blockSizeX_ {width_ / numberOfBlocks_};
  int blockSizeY_ {height_ / numberOfBlocks_};
  MobileRT::Scene scene_ {};
  std::unique_ptr<MobileRT::Sampler> samplerCamera {};
  std::unique_ptr<MobileRT::Sampler> samplerPixel {};
  std::unique_ptr<MobileRT::Shader> shader_ {};
  std::unique_ptr<MobileRT::Camera> camera {};
  switch (scene) {
    case 0:
      camera = std::make_unique<Components::Perspective>(
        MobileRT::Point3D{0.0f, 0.0f, - 3.4f},
        MobileRT::Point3D{0.0f, 0.0f, 1.0f},
        MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
        45.0f, 45.0f * ratio);
      scene_ = cornellBoxScene(std::move(scene_));
      break;

    case 1:
      camera = std::make_unique<Components::Orthographic>(
        MobileRT::Point3D{0.0f, 1.0f, - 10.0f},
        MobileRT::Point3D{0.0f, 1.0f, 7.0f},
        MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
        10.0f, 10.0f);
      /*camera = std::make_unique<Components::Perspective>(
        MobileRT::Point3D(0.0f, 0.5f, 1.0f),
        MobileRT::Point3D(0.0f, 0.0f, 7.0f),
        MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
        60.0f, 60.0f * ratio);*/
      scene_ = spheresScene(std::move(scene_));
      break;

    case 2:
      camera = std::make_unique<Components::Perspective>(
        MobileRT::Point3D {0.0f, 0.0f, - 3.4f},
        MobileRT::Point3D {0.0f, 0.0f, 1.0f},
        MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
        45.0f, 45.0f * ratio);
      scene_ = cornellBoxScene2(std::move(scene_), samplesLight, samplesPixel, width_, height_);
      break;

    case 3:
      camera = std::make_unique<Components::Perspective>(
        MobileRT::Point3D {0.0f, 0.5f, 1.0f},
        MobileRT::Point3D {0.0f, 0.0f, 7.0f},
        MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
        60.0f, 60.0f * ratio);
      scene_ = spheresScene2(std::move(scene_));
      break;

    default:
      objLoader.fillScene(&scene_);
      camera = std::make_unique<Components::Perspective>(
        MobileRT::Point3D {0.0f, 0.5f, 3.0f},
        MobileRT::Point3D {0.0f, 0.5f, - 1.0f},
        MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
        45.0f, 45.0f * ratio);
      break;
  }
  switch (sampler) {
    case 1:
      if (samplesPixel > 1) {
        samplerPixel = std::make_unique<Components::HaltonSeq>(
          static_cast<unsigned>(width_) * static_cast<unsigned>(height_) * 2 *
          static_cast<uint64_t>(samplesPixel), 1);
      } else {
        samplerPixel = std::make_unique<Components::Constant>(0.5f);
      }
      samplerCamera = std::make_unique<Components::HaltonSeq>(
        width_, height_, samplesPixel,
        blockSizeX_, blockSizeY_);
      break;

    default:
      if (samplesPixel > 1) {
        samplerPixel = std::make_unique<Components::Stratified>(
          static_cast<unsigned>(width_) * static_cast<unsigned>(height_) * 2 *
          static_cast<uint64_t>(samplesPixel), 1);
      } else {
        samplerPixel = std::make_unique<Components::Constant>(0.5f);
      }
      samplerCamera = std::make_unique<Components::Stratified>(
        width_, height_, samplesPixel,
        blockSizeX_, blockSizeY_);
      break;
  }
  const uint64_t domainRay {
    (static_cast<unsigned>(width_) * static_cast<unsigned>(height_) * 2) *
    static_cast<uint64_t>(samplesPixel) * RayDepthMax};
  const uint64_t domainLight {
    (static_cast<unsigned>(width_) * static_cast<unsigned>(height_) * 2) *
    static_cast<uint64_t>(samplesPixel) * RayDepthMax * static_cast<uint64_t>(samplesLight)};
  switch (shader) {
    case 1: {
      shader_ = std::make_unique<Components::Whitted>(std::move(scene_), samplesLight);
      break;
    }

    case 2: {
      LOG("domainRay = ", domainRay, "domainLight = ", domainLight);
      //std::unique_ptr<MobileRT::Sampler> samplerRay(std::make_unique<Components::HaltonSeq>(domainRay, 1));
      std::unique_ptr<MobileRT::Sampler> samplerRay(
        std::make_unique<Components::MersenneTwister>(domainRay, 1));
      //std::unique_ptr<MobileRT::Sampler> samplerLight(std::make_unique<Components::HaltonSeq>(domainLight, 1));
      std::unique_ptr<MobileRT::Sampler> samplerLight(
        std::make_unique<Components::MersenneTwister>(domainLight, 1));
      //std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette(std::make_unique<Components::HaltonSeq>(domainLight, 1));
      std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette(
        std::make_unique<Components::MersenneTwister>(domainLight, 1));
      shader_ = std::make_unique<Components::PathTracer>(
        std::move(scene_), std::move(samplerRay), std::move(samplerLight),
        std::move(samplerRussianRoulette), samplesLight);
      break;
    }

    case 3: {
      shader_ = std::make_unique<Components::DepthMap>(std::move(scene_), MobileRT::Point3D(1, 1, 1));
      break;
    }

    case 4: {
      shader_ = std::make_unique<Components::DiffuseMaterial>(std::move(scene_));
      break;
    }

    default: {
      shader_ = std::make_unique<Components::NoShadows>(std::move(scene_), samplesLight);
      break;
    }
  }
  renderer_ = new MobileRT::Renderer {
    std::move(samplerCamera), std::move(shader_), std::move(camera),
    static_cast<unsigned>(width_), static_cast<unsigned>(height_),
    static_cast<unsigned>(blockSizeX_), static_cast<unsigned>(blockSizeY_),
    std::move (samplerPixel)};

  LOG("x = ", blockSizeX_, "[", width_, "]");
  LOG("y = ", blockSizeY_, "[", height_, "]");
  LOG("TRIANGLES = ", renderer_->shader_->scene_.triangles_.size());
  LOG("SPHERES = ", renderer_->shader_->scene_.spheres_.size());
  LOG("PLANES = ", renderer_->shader_->scene_.planes_.size());
  LOG("LIGHTS = ", renderer_->shader_->scene_.lights_.size());
  LOG("MATERIALS = ", renderer_->shader_->scene_.materials_.size());
}

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *env,
        jobject /*thiz*/,
        jobject dstBitmap,
        jint const nThreads) noexcept {

  void *dstPixels {nullptr};
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
    AndroidBitmap_unlockPixels(env, dstBitmap);
  working_ = State::BUSY;
    LOG("WORKING = BUSY");
    thread_ = new std::thread(thread_work, dstPixels, nThreads);
}

extern "C"
State Java_puscas_mobilertapp_DrawView_redraw (
        JNIEnv *env,
        jobject /*thiz*/,
        jobject dstBitmap) noexcept {

  void *dstPixels {nullptr};
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

  const float u {static_cast<float> (jx) / width_};
  const float v {static_cast<float> (jy) / height_};
  MobileRT::Ray ray {renderer_ -> camera_ -> generateRay (u, v, 0.0f, 0.0f)};
  MobileRT::Intersection intersection {};
  const int primitiveID(renderer_->shader_->traceTouch(&intersection, std::move(ray)));
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

  const float u {static_cast<float> (jx) / width_};
  const float v {static_cast<float> (jy) / height_};
  const MobileRT::Ray ray {renderer_ -> camera_ -> generateRay (u, v, 0.0f, 0.0f)};
  const uint32_t index {static_cast<uint32_t>(primitiveIndex)};
  const MobileRT::Plane plane {
    MobileRT::Point3D{0.0f, 0.0f, renderer_ -> shader_ -> scene_ . planes_[index] . getZ ()},
    MobileRT::Vector3D{0.0f, 0.0f, - 1.0f}};
  MobileRT::Intersection intersection {};
    plane.intersect(&intersection, ray);
    renderer_->shader_->scene_.planes_[index].moveTo(intersection.point_.x_,
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
int Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv * /*env*/,
        jobject /*thiz*/,
        jint const size
) noexcept {
    return roundDownToMultipleOf(size, numberOfBlocks_);
}
