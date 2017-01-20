//
// Created by Tiago on 16-10-2016.
//

#include "Renderer.h"

using namespace MobileRT;

Renderer::Renderer(Sampler &sampler) :
        sampler_(&sampler)
{
}

Renderer::~Renderer() {
}

void Renderer::renderFrame(unsigned int *const bitmap,
                           const unsigned int numThreads) const
{
    this->sampler_->resetTask();
    std::thread *const threads(new std::thread[numThreads - 1]);
    for (unsigned int i (0); i < numThreads - 1; i++)
    {
        threads[i] = std::thread(&Sampler::renderScene, this->sampler_, bitmap, i, numThreads);
    }
    this->sampler_->renderScene(bitmap, numThreads - 1, numThreads);
    for (unsigned int i (0); i < numThreads - 1; i++)
    {
        threads[i].join();
    }
    delete[] threads;
}

void Renderer::stopRender() const {
    this->sampler_->stopSampling();
}