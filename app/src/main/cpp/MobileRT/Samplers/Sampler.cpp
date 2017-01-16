//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"

using namespace MobileRT;

Sampler::Sampler(
        const unsigned int width, const unsigned int height,
        const Shader &shader, const unsigned int samples,
        const Perspective &camera, const Scene &scene) :
        width_(width),
        height_(height),
        samples_(samples),
        deviationIncrement_(1.0f / this->samples_),
        taskLine_(1),
        accumulate_(new RGB[this->width_ * this->height_]),
        tasks_(this->width_ * this->height_ * this->samples_),
        maxHalton_(roundToPower2(this->tasks_)),
        rayTracer_(new RayTracer(scene, shader)),
        camera_(&camera)
{
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

const unsigned int Sampler::getTasks(const unsigned int tasks, const int sample) {
    const unsigned int task(this->taskLine_.fetch_add(tasks, std::memory_order_relaxed));
    return task - sample * this->height_;
}

void Sampler::resetTask() {
    this->taskLine_ = 1;
}

void Sampler::stopSampling() {
    this->height_ = 0;
    this->tasks_ = 0;
    this->maxHalton_ = 0;
}