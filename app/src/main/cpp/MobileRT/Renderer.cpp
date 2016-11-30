//
// Created by Tiago on 16-10-2016.
//

#include "Renderer.h"
#include "Samplers/Stratified.h"
#include "Samplers/Jittered.h"

using namespace MobileRT;

Renderer::Renderer(const unsigned int width, const unsigned int height,
                   const unsigned int whichScene, const unsigned int whichShader,
                   const unsigned int whichSampler, const unsigned int samples)
{
    switch (whichSampler)
    {
        case 0 :
            this->sampler_ = new Stratified (width, height, whichScene, whichShader, samples);
            break;

        case 1 :
            this->sampler_ = new Jittered (width, height, whichScene, whichShader, samples);
            break;

        default:
            this->sampler_ = nullptr;
            break;
    }
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
    delete this->sampler_;
}

void Renderer::stopRender() const {
    this->sampler_->stopSampling();
}