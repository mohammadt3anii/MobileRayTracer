//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"

using namespace MobileRT;

Sampler::Sampler(const unsigned int width, const unsigned int height,
                 const unsigned int shader, const unsigned int samples,
                 const Perspective &camera, const Scene &scene) :
        width_(width),
        height_(height),
        samples_(samples),
        deviationIncrement_(1.0f / this->samples_),
        taskLine_(0),
        accumulate_(new RGB[this->width_ * this->height_])
{
    this->camera_ = &camera;
    this->rayTracer_ = new RayTracer(scene, shader);
}

Sampler::~Sampler ()
{
    delete this->rayTracer_;
    delete[] this->accumulate_;
}

const float Sampler::deviation(const int index) const
{
    return (-0.5f + (index * this->deviationIncrement_) - (this->deviationIncrement_ * 0.5f));
}

const unsigned int Sampler::getTask(const int sample) {
    //unsigned int task = this->taskLine_.fetch_add(1, std::memory_order_relaxed);
    this->mutex_.lock();
    const unsigned int task = this->taskLine_++;
    this->mutex_.unlock();
    return task - sample * this->height_;
}

void Sampler::resetTask() {
    this->taskLine_ = 0;
}

void Sampler::stopSampling() {
    this->width_ = 0;
    this->height_ = 0;
    this->samples_ = 0;
}