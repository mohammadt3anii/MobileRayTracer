//
// Created by Tiago on 16-10-2016.
//

#include "Renderer.h"

using namespace MobileRT;

Renderer::Renderer(const Shader &shader, Sampler &sampler,
                   const Perspective &camera, const Scene &scene, const unsigned int samples) :
        sampler_(&sampler)
{
}

Renderer::~Renderer() {
}

void Renderer::render(unsigned int *canvas,
                      const unsigned int numThreads) const
{
    this->sampler_->resetTask();
    std::thread *threads = new std::thread[numThreads - 1];
    for (unsigned int i (0); i < numThreads - 1; i++)
    {
        threads[i] = std::thread(&Sampler::renderScene, this->sampler_, canvas, i, numThreads);
    }
    this->sampler_->renderScene(canvas, numThreads - 1, numThreads);
    for (unsigned int i (0); i < numThreads - 1; i++)
    {
        threads[i].join();
    }
    delete[] threads;
}

void Renderer::stopRender() const {
    this->sampler_->stopSampling();
}