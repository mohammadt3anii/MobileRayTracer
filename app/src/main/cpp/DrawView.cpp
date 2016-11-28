//
// Created by Tiago on 14-10-2016.
//

#include "MobileRT/All.h"
#include <jni.h>
#include <thread>
#include <android/bitmap.h>
//#include <android/log.h>

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2
};
static MobileRT::Renderer *renderer_(nullptr);
static int working_(IDLE);

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

void thread_work(void *dstPixels, unsigned int numThreads) {
    renderer_->render(static_cast<unsigned int *>(dstPixels), numThreads);
    working_ = FINISHED;
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
    renderer_ = new MobileRT::Renderer(static_cast<unsigned int>(width),
                                       static_cast<unsigned int>(height),
                                       static_cast<unsigned int>(scene),
                                       static_cast<unsigned int>(shader),
                                       static_cast<unsigned int>(sampler),
                                       static_cast<unsigned int> (samples));
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv *env,
        jobject,//this,
        jobject dstBitmap,
        jint nThreads
) {
    //__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Threads = %d \n", numThreads_);
    void *dstPixels;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

    working_ = BUSY;
    std::thread(thread_work, dstPixels, static_cast<unsigned int> (nThreads)).detach();

    AndroidBitmap_unlockPixels(env, dstBitmap);
}