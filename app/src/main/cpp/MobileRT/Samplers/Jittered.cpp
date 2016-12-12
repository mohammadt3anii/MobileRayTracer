//
// Created by Tiago on 21-11-2016.
//

#include "Jittered.h"
#include "../Utils.h"
#include "../Color_Models/ToneMapper.h"
#include <cstdlib>
#include <cmath>
//#include <random>

#include <android/log.h>

using namespace MobileRT;

Jittered::Jittered(
        const unsigned int width, const unsigned int height,
        const unsigned int shader, const unsigned int samples,
        const Perspective &camera, const Scene &scene) :
        Sampler(width, height, shader, samples, camera, scene)
{
}

bool Jittered::finishSampling(unsigned int &task) const {
    for (; task < this->tasks_; task++) {
        if (this->accumulate_[task].samples_ < this->samples_) {
            return false;
        }
    }
    return true;
}

void Jittered::renderScene(unsigned int *canvas) {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelWidth(0.5f / this->width_);
    const float pixelHeight(0.5f / this->height_);
    const float half_rand_max(RAND_MAX * 0.5f);
    //const float half_rand_max((std::pow(2,96)-1) * 0.5f);
    RGB rayRGB;
    RGB average;
    Intersection intersection;
    Vector3D vector;
    Ray ray;

    unsigned int missed(0);
    for (unsigned int taskId(getTask(0)); taskId < this->maxHalton_; taskId = getTask(0))
    {
        const double halton(haltonSequence(taskId, 2));
        const unsigned int i = static_cast<unsigned int>(round(halton * this->maxHalton_));
        if (i >= this->tasks_) continue;
        const unsigned int ii = i / this->samples_;

        RGB &accumulate = this->accumulate_[ii];
        if (accumulate.samples_ == this->samples_) {
            missed++;
            continue;
        }

        const unsigned int x = ii % this->width_;
        const unsigned int y = ii / this->width_;

        const float v_alpha(fastArcTan(this->camera_->vFov_ * (0.5f - (y * INV_IMG_HEIGHT))));

        // generate the ray
        const float u_alpha(fastArcTan(this->camera_->hFov_ * ((x * INV_IMG_WIDTH) - 0.5f)));
        const float randU(std::rand() / half_rand_max - 1.0f);
        const float randV(std::rand() / half_rand_max - 1.0f);
        const float u_alpha_jittered(u_alpha + (randU * pixelWidth));
        const float v_alpha_jittered(v_alpha + (randV * pixelHeight));
        //builds ray and puts it in ray variable
        this->camera_->getRay(ray, u_alpha_jittered, v_alpha_jittered);
        //ray trace and puts the color in rayRGB variable
        this->rayTracer_->rayTrace(rayRGB, ray, intersection, vector);
        accumulate.addSample(average, rayRGB);
        average.average();
        // toneMap and convert to Paint
        canvas[ii] = ToneMapper::RGB2Color(average);

        /*if (accumulate.samples_ == this->samples_ &&
                finishSampling(taskFinished)) {
            stopSampling();
            return;
        }*/
    }

    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "MISSED=%u", missed);
}