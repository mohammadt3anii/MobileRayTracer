//
// Created by Tiago on 14-10-2016.
//

#include "DrawView.hpp"

static State working_ {State::IDLE};
static MobileRT::Renderer *renderer_ {nullptr};
static std::thread *thread_ {nullptr};
static int width_ {0};
static int height_ {0};
static float fps_ {0.0f};
static int64_t timeFrame_ {0};
static int64_t numberOfLights_ {0};

static std::string
readTextAsset (JNIEnv *const env, const jobject assetManager, const char *const filename) {
  AAssetManager *const mgr {AAssetManager_fromJava (env, assetManager)};
  std::vector<char> buffer {};
  AAsset *const asset {AAssetManager_open (mgr, filename, AASSET_MODE_STREAMING)};

  //holds size of searched file
  const off64_t length {AAsset_getLength64 (asset)};
  //keeps track of remaining bytes to read
  off64_t remaining {AAsset_getRemainingLength64 (asset)};
  const size_t Mb {1000 * 1024}; // 1Mb is maximum chunk size for compressed assets
  size_t currChunk {};
  buffer.reserve (static_cast<size_t>(length));

  //while we have still some data to read
  while (remaining != 0) {
    //set proper size for our next chunk
    if (remaining >= static_cast<off64_t>(Mb)) {
      currChunk = Mb;
    } else {
      currChunk = static_cast<size_t>(remaining);
    }
    char *chunk {new char[currChunk]};
    //read data chunk
    if (AAsset_read (asset, chunk, currChunk) > 0) // returns less than 0 on error
    {
      //and append it to our vector
      buffer.insert (buffer.end (), chunk, chunk + currChunk);
      remaining = AAsset_getRemainingLength64 (asset);
    }
    delete[] chunk;
  }
  AAsset_close (asset);
  const std::string res {buffer.begin (), buffer.end ()};
  return res;
}

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

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
  JNIEnv *const env,
  jobject /*thiz*/,
  jobject dstBitmap
) noexcept {
  thread_->join();
  delete thread_;
  delete renderer_;

  timeFrame_ = 0;
  fps_ = 0.0f;
  AndroidBitmap_unlockPixels(env, dstBitmap);
  working_ = State::IDLE;
  LOG("WORKING = IDLE");
}

extern "C"
State Java_puscas_mobilertapp_DrawView_isWorking (
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
int64_t Java_puscas_mobilertapp_DrawView_initialize (
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


  int64_t res = [&] () -> int64_t {
    renderer_ = nullptr;
    const float ratio {
      width_ > height_ ? static_cast<float>(width_) / height_ : static_cast<float>(height_) / width_};
    const float hfovFactor {width_ > height_ ? ratio : 1.0f};
    const float vfovFactor {width_ < height_ ? ratio : 1.0f};
    MobileRT::Scene scene_ {};
    std::unique_ptr<MobileRT::Sampler> samplerPixel {};
    std::unique_ptr<MobileRT::Shader> shader_ {};
    std::unique_ptr<MobileRT::Camera> camera {};
    MobileRT::Point3D maxDist;
    switch (scene) {
      case 0:
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D{0.0f, 0.0f, -3.4f},
          MobileRT::Point3D{0.0f, 0.0f, 1.0f},
          MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
          45.0f * hfovFactor, 45.0f * vfovFactor);
        scene_ = cornellBoxScene (std::move (scene_));
        maxDist = MobileRT::Point3D {1, 1, 1};
        break;

      case 1:
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D{4.0f, 4.0f, -8.0f},
          MobileRT::Point3D{4.0f, 4.0f, 4.0f},
          MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
          45.0f * hfovFactor, 45.0f * vfovFactor);
        /*camera = std::make_unique<Components::Orthographic> (
          MobileRT::Point3D{0.0f, 1.0f, - 10.0f},
          MobileRT::Point3D{0.0f, 1.0f, 7.0f},
          MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
          10.0f * hfovFactor, 10.0f * vfovFactor);*/
        /*camera = std::make_unique<Components::Perspective>(
          MobileRT::Point3D(0.0f, 0.5f, 1.0f),
          MobileRT::Point3D(0.0f, 0.0f, 7.0f),
          MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
          60.0f * hfovFactor, 60.0f * vfovFactor);*/
        scene_ = spheresScene (std::move (scene_));
        maxDist = MobileRT::Point3D {8, 8, 8};
        break;

      case 2:
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D {0.0f, 0.0f, -3.4f},
          MobileRT::Point3D {0.0f, 0.0f, 1.0f},
          MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
          45.0f * hfovFactor, 45.0f * vfovFactor);
        scene_ = cornellBoxScene2 (std::move (scene_));
        maxDist = MobileRT::Point3D {1, 1, 1};
        break;

      case 3:
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D {0.0f, 0.5f, 1.0f},
          MobileRT::Point3D {0.0f, 0.0f, 7.0f},
          MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
          60.0f * hfovFactor, 60.0f * vfovFactor);
        scene_ = spheresScene2 (std::move (scene_));
        maxDist = MobileRT::Point3D {8, 8, 8};
        break;

      default: {
        jboolean isCopy (JNI_FALSE);
        const char *objFileName {(env)->GetStringUTFChars (objFile, &isCopy)};
        const char *matFileName {(env)->GetStringUTFChars (matFile, &isCopy)};
        std::string obj {readTextAsset (env, assetManager, objFileName)};
        std::string mat {readTextAsset (env, assetManager, matFileName)};
        Components::OBJLoader objLoader {obj.c_str (), mat.c_str ()};
        env->ReleaseStringUTFChars (objFile, objFileName);
        env->ReleaseStringUTFChars (matFile, matFileName);
        objLoader.process ();
        if (!objLoader.isProcessed ()) {
          exit (0);
        }
        objLoader.fillScene (&scene_, [](){return std::make_unique<Components::StaticHaltonSeq> ();});
        /*const MobileRT::Material lightMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           1.0f,
                                           MobileRT::RGB {0.9f, 0.9f, 0.9f}};
        scene_.lights_.emplace_back (new Components::PointLight {
          lightMat, MobileRT::Point3D {0.0f, 0.9f, 0.0f}});
        //cornell spheres
        camera = std::make_unique<Components::Perspective> (MobileRT::Point3D {0.0f, 0.7f, 3.0f},
                                                            MobileRT::Point3D {0.0f, 0.7f, -1.0f},
                                                            MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
                                                            45.0f * hfovFactor, 45.0f * vfovFactor);*/
        //teapot
        /*camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D {0.0f, 30.0f, -200.0f}, MobileRT::Point3D {0.0f, 30.0f, 100.0f},
          MobileRT::Vector3D {0.0f, 1.0f, 0.0f}, 45.0f * hfovFactor, 45.0f * vfovFactor);*/
        //conference
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D {730.0f, 600.0f, -950.0f},
          MobileRT::Point3D {400.0f, 300.0f, 0.0f},
          MobileRT::Vector3D {0.0f, 1.0f, 0.0f}, 45.0f * hfovFactor, 45.0f * vfovFactor);
        const MobileRT::Material lightMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           1.0f,
                                           MobileRT::RGB {0.001f, 0.001f, 0.001f}};
        scene_.lights_.emplace_back (new Components::PointLight {
          lightMat, MobileRT::Point3D {400.0f, 500.0f, -500.0f}});
        maxDist = MobileRT::Point3D {1, 1, 1};
      }
        break;
    }
    if (samplesPixel > 1) {
      samplerPixel = std::make_unique<Components::StaticHaltonSeq> ();
    } else {
      samplerPixel = std::make_unique<Components::Constant> (0.5f);
    }
    switch (shader) {
      case 1: {
        shader_ = std::make_unique<Components::Whitted> (std::move (scene_), samplesLight,
                                                         MobileRT::Shader::Accelerator (
                                                           accelerator));
        break;
      }

      case 2: {
        std::unique_ptr<MobileRT::Sampler> samplerRay {
          std::make_unique<Components::StaticHaltonSeq> ()};
        std::unique_ptr<MobileRT::Sampler> samplerLight {
          std::make_unique<Components::StaticHaltonSeq> ()};
        std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {
          std::make_unique<Components::StaticHaltonSeq> ()};

        shader_ = std::make_unique<Components::PathTracer> (
          std::move (scene_), std::move (samplerRay), std::move (samplerLight),
          std::move (samplerRussianRoulette), samplesLight,
          MobileRT::Shader::Accelerator (accelerator));
        break;
      }

      case 3: {
        shader_ = std::make_unique<Components::DepthMap> (std::move (scene_), maxDist,
                                                          MobileRT::Shader::Accelerator (
                                                            accelerator));
        break;
      }

      case 4: {
        shader_ = std::make_unique<Components::DiffuseMaterial> (std::move (scene_),
                                                                 MobileRT::Shader::Accelerator (
                                                                   accelerator));
        break;
      }

      default: {
        shader_ = std::make_unique<Components::NoShadows> (std::move (scene_), samplesLight,
                                                           MobileRT::Shader::Accelerator (
                                                             accelerator));
        break;
      }
    }
    renderer_ = new MobileRT::Renderer {
      std::move (shader_), std::move (camera), std::move (samplerPixel),
      static_cast<unsigned>(width_), static_cast<unsigned>(height_),
      static_cast<unsigned>(samplesPixel)};
    const int64_t triangles {static_cast<int64_t> (renderer_->shader_->scene_.triangles_.size ())};
    const int64_t spheres {static_cast<int64_t> (renderer_->shader_->scene_.spheres_.size ())};
    const int64_t planes {static_cast<int64_t> (renderer_->shader_->scene_.planes_.size ())};
    const int64_t rectangles {static_cast<int64_t> (renderer_->shader_->scene_.rectangles_.size ())};
    numberOfLights_ = static_cast<int64_t> (renderer_->shader_->scene_.lights_.size ());
    const int64_t nPrimitives = triangles + spheres + planes + rectangles;
    LOG("TRIANGLES = ", triangles);
    LOG("SPHERES = ", spheres);
    LOG("PLANES = ", planes);
    LOG("RECTANGLES = ", rectangles);
    LOG("LIGHTS = ", numberOfLights_);
    return nPrimitives;
  }();


  LOG("PRIMITIVES = ", res);
  return res;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
  JNIEnv *const env,
  jobject /*thiz*/,
  jobject dstBitmap,
  jint const nThreads) noexcept {
  working_ = State::BUSY;
  LOG("WORKING = BUSY");
  void *dstPixels {nullptr};
  AndroidBitmap_lockPixels (env, dstBitmap, &dstPixels);
  thread_ = new std::thread (
    [] (unsigned *pixels, const int numOfThreads) {
      int rep {1};
      do {
        const std::chrono::steady_clock::time_point start {std::chrono::steady_clock::now ()};
        renderer_->renderFrame (pixels, numOfThreads);
        timeFrame_ = std::chrono::duration_cast<std::chrono::milliseconds> (
          std::chrono::steady_clock::now () - start).count ();
        FPS ();
        renderer_->camera_->position_.x_ += 1.0f;
      } while (working_ != State::STOPPED && rep-- > 1);
      if (working_ != State::STOPPED) {
        working_ = State::FINISHED;
        LOG("WORKING = FINISHED");
      }
    }, static_cast<unsigned *>(dstPixels), static_cast<int>(nThreads)
  );
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
  //const int primitiveID {renderer_->shader_->traceTouch(&intersection, std::move(ray))};
  const int primitiveID {-1};
  return primitiveID;
}

extern "C"
void Java_puscas_mobilertapp_DrawView_moveTouch(
  JNIEnv *const /*env*/,
  jobject /*thiz*/,
  jfloat const jx,
  jfloat const jy,
  jint const primitiveIndex
) noexcept {
  const float u {static_cast<float> (jx) / width_};
  const float v {static_cast<float> (jy) / height_};
  const MobileRT::Ray ray {renderer_->camera_->generateRay (u, v, 0.0f, 0.0f)};
  const uint32_t index {static_cast<uint32_t>(primitiveIndex)};
  const MobileRT::Plane plane {
    MobileRT::Point3D{0.0f, 0.0f, renderer_->shader_->scene_.planes_[index].shape_.getZ ()},
    MobileRT::Vector3D{0.0f, 0.0f, - 1.0f}};
  MobileRT::Intersection intersection {};
  plane.intersect(&intersection, ray);
  renderer_->shader_->scene_.planes_[index].shape_.moveTo (intersection.point_.x_,
                                                           intersection.point_.y_);
}

extern "C"
float Java_puscas_mobilertapp_DrawView_getFPS(
  JNIEnv *const /*env*/,
  jobject /*thiz*/
) noexcept {
  return fps_;
}

extern "C"
int64_t Java_puscas_mobilertapp_DrawView_getTimeFrame(
  JNIEnv *const /*env*/,
  jobject /*thiz*/
) noexcept {
  return timeFrame_;
}

extern "C"
unsigned Java_puscas_mobilertapp_DrawView_getSample (
  JNIEnv *const /*env*/,
  jobject /*thiz*/
) noexcept {
  return renderer_->getSample();
}

extern "C"
int Java_puscas_mobilertapp_DrawView_resize(
  JNIEnv *const /*env*/,
  jobject /*thiz*/,
  jint const size
) noexcept {
  return roundDownToMultipleOf (size, static_cast<int>(std::sqrt (NumberOfBlocks)));
}

extern "C"
int64_t Java_puscas_mobilertapp_DrawView_getNumberOfLights (
  JNIEnv *const /*env*/,
  jobject /*thiz*/
) noexcept {
  return numberOfLights_;
}
