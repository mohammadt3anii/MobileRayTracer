//
// Created by puscas on 14-10-2016.
//

#include "DrawView.h"
#include <jni.h>
#include <android/bitmap.h>
#include <pthread.h>

/**
 * Draws something into the given bitmap
 * @param  env
 * @param  thiz
 * @param  dstBitmap   The bitmap to place the results
 * @param  width       The bitmap with
 * @param  height      The bitmap height
 * @param  elapsedTime The number of milliseconds since the app was started
 */

enum State {
    IDLE = 0, BUSY = 1, FINISHED = 2
};
static Renderer *renderer_ = NULL;
static void *dstPixels_ = NULL;
static pthread_t thread_handle_;
static int width_ = 0;
static int working_ = IDLE;
static unsigned int numThreads_ = 1;
/*static JNIEnv* env_ = NULL;
static jobject dstBitmap_ = NULL;
static JavaVM* jvm_ = NULL;*/

extern "C"
int Java_com_example_puscas_mobileraytracer_DrawView_isWorking() {
    return working_;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_finished() {
    working_ = IDLE;
}

void *thread_work(void *args) {/*
    jobject* kik = (jobject*) args;
    JNIEnv* myNewEnv;
    jvm_->AttachCurrentThread(&myNewEnv, NULL);
    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "1");*/

    renderer_->render(static_cast<unsigned int *>(dstPixels_), numThreads_);
    working_ = FINISHED;

    //jvm_->DetachCurrentThread();
    return NULL;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_initialize(
        JNIEnv *,// env,
        jobject, //thiz,
        jint whichScene,
        jint whichShader,
        jint width,
        jint height
) {
    working_ = IDLE;
    width_ = width;
    renderer_ = new Renderer(width, height, whichScene, whichShader);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    //jvm_ = jvm;
    return JNI_VERSION_1_6;
}

extern "C"
void Java_com_example_puscas_mobileraytracer_DrawView_drawIntoBitmap(
        JNIEnv *env,
        jobject,//thiz,
        jobject dstBitmap,
        jint nThreads
) {
    /* Get a reference to jctf object's class */
    //jclass object = env->GetObjectClass(thiz);
    //jclass object = env->FindClass("com/example/puscas/mobileraytracer/DrawViewImpl");
    /* Get the Field ID of the instance variables "jni_result" */
    //jfieldID fidError = env->GetFieldID(object, "finish_", "Z");
    //jboolean p = env->GetBooleanField(object, fidError);
    /* Change the variable "jni_result" */
    //jboolean newValue = (jboolean) true;
    //env->SetBooleanField(object, fidError, newValue);

    //dstBitmap_ = dstBitmap;

    numThreads_ = nThreads;
    // Grab the dst bitmap info and pixels
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels_);

    // JNIEnv* env; (initialized somewhere else)
    working_ = BUSY;
    pthread_create(&thread_handle_, NULL, thread_work, NULL);
    //pthread_join(thread_handle, NULL);

    // Unlock the dst's pixels
    AndroidBitmap_unlockPixels(env, dstBitmap);
}