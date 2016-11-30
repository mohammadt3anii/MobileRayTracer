//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"
#include "../Scenes/Cornell.h"
#include "../Scenes/Spheres.h"

using namespace MobileRT;

Sampler::Sampler(const unsigned int width, const unsigned int height,
                 const unsigned int scene, const unsigned int shader,
                 const unsigned int samples) :
        width_(width),
        height_(height),
        samples_(samples),
        deviationIncrement_(1.0f / this->samples_),
        taskLine_(0),
        accumulate_(new RGB[this->width_ * this->height_])
{
    const float ratio = static_cast<float>(height_) / static_cast<float>(width_);

    // create and load the Scene, parameterize the camera
    switch (scene)
    {
        case 0 : // cornell
        {
            this->scene_ = new Cornell();
            const float hFov = 45.0f;
            const float vFov = hFov * ratio;
            this->camera_ = new Perspective(Point3D(0.0f, 0.0f, -3.4f), hFov, vFov);
        }
            break;

        case 1 : // spheres
        {
            this->scene_ = new Spheres();
            const float hFov = 60.0f;
            const float vFov = hFov * ratio;
            this->camera_ = new Perspective(Point3D(0.0f, 0.5f, 1.0f), hFov, vFov);
        }
            break;

        default:
            break;
    }
    // create the ray tracer
    this->rayTracer_ = new RayTracer(*this->scene_, shader);
}

Sampler::~Sampler ()
{
    delete this->scene_;
    delete this->camera_;
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