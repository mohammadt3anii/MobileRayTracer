//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"

using namespace MobileRT;

Sampler::Sampler(
        const unsigned int width, const unsigned int height,
        const Shader &shader, const unsigned int samples,
        const Camera &camera) :
        width_(width),
        height_(height),
        shader_(shader),
        camera_(camera),
        samples_(samples),
        deviationIncrement_(1.0f / this->samples_),
        taskLine_(1),
        accumulate_(new RGB[this->width_ * this->height_]),
        tasks_(this->width_ * this->height_ * this->samples_),
        maxHalton_(roundToPower2(this->tasks_))
{
}

Sampler::~Sampler ()
{
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
    const int size(this->width_ * this->height_);
    for (int i(0); i < size; i++) {
        this->accumulate_[i].reset();
    }
}

void Sampler::stopRender() {
    this->height_ = 0;
    this->tasks_ = 0;
    this->maxHalton_ = 0;
}

void Sampler::renderFrame(unsigned int *const bitmap, const unsigned int numThreads) {
    resetTask();
    std::thread *const threads(new std::thread[numThreads - 1]);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i] = std::thread(&Sampler::renderScene, this, bitmap, i, numThreads);
    }
    renderScene(bitmap, numThreads - 1, numThreads);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i].join();
    }
    delete[] threads;
}