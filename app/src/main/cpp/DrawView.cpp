//
// Created by Tiago on 14-10-2016.
//

#include "MobileRT/All.h"
#include "Scenes.h"
#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

static int working_(IDLE);
static const MobileRT::Scene *sceneT_(nullptr);
static const MobileRT::Perspective *camera_(nullptr);
static const MobileRT::Renderer *renderer_(nullptr);

void freeMemory() {
    delete camera_;
    delete sceneT_;
}

extern "C"
int Java_com_example_puscas_mobileraytracer_DrawView_isWorking(
        JNIEnv *,// env,
        jobject //this,
) {
    return working_;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_finished(
        JNIEnv *,// env,
        jobject//this,
) {
    working_ = IDLE;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_stop(
        JNIEnv *,// env,
        jobject //this,
) {
    renderer_->stopRender();
    working_ = STOPPED;
}

void thread_work(void *dstPixels, unsigned int numThreads) {
    renderer_->render(static_cast<unsigned int *>(dstPixels), numThreads);
    working_ = FINISHED;
    freeMemory();
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_initialize(
        JNIEnv *,// env,
        jobject, //this,
        jint scene,
        jint shader,
        jint width,
        jint height,
        jint sampler,
        jint samples
) {
    working_ = IDLE;
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
    renderer_ = new MobileRT::Renderer(static_cast<unsigned int>(width),
                                       static_cast<unsigned int>(height),
                                       static_cast<unsigned int>(shader),
                                       static_cast<unsigned int>(sampler),
                                       *camera_,
                                       *sceneT_,
                                       static_cast<unsigned int>(samples));
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap,
        jint nThreads
) {
    int number = 5;
    for (int i = 0; i <= 4; i++) {
        __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Sequence = %d \n",
                            static_cast<int> (haltonSequence(i, 2) * number));
    }
    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

    working_ = BUSY;
    std::thread(thread_work, dstPixels, static_cast<unsigned int> (nThreads)).detach();

    AndroidBitmap_unlockPixels(env, dstBitmap);
}