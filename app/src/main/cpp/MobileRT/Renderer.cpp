//
// Created by Tiago on 16-10-2016.
//

#include "Renderer.h"
#include "SceneCornell.h"
#include "SceneSpheres.h"
#include "ToneMapper.h"
#include <thread>
#include <iostream>

using namespace MobileRT;

Renderer::Renderer(const unsigned int width, const unsigned int height,
                   const unsigned int whichScene, const unsigned int whichShader) :
        width_(width),
        height_(height)
{
    const float ratio = static_cast<float>(height_) / static_cast<float>(width_);

    // create and load the Scene, parameterize the camera
    switch (whichScene)
    {
        case 0 : // cornell
        {
            this->scene_ = new SceneCornell();
            const float hFov = 45.0f;
            const float vFov = hFov * ratio;
            this->camera_ = new RTCamera(Point(0.0f, 0.0f, -3.4f), hFov, vFov);
        }
            break;

        case 1 : // spheres
        {
            this->scene_ = new SceneSpheres();
            const float hFov = 60.0f;
            const float vFov = hFov * ratio;
            this->camera_ = new RTCamera(Point(0.0f, 0.5f, 1.0f), hFov, vFov);
        }
            break;

        default:
            break;
    }
    // create the ray tracer
    this->rTracer_ = new RayTrace(*this->scene_, whichShader);
}

void Renderer::thread_render(unsigned int *canvas, unsigned int tid,
                                 unsigned int numThreads) const {
    const float width (this->width_);
    const float height (this->height_);
    const float INV_IMG_WIDTH (1.0f / width);
    const float INV_IMG_HEIGHT (1.0f / height);
    Ray ray;
    RGB rayRGB;
    Intersection isect;
    for (unsigned int y = tid; y < height; y += numThreads)
    {
        const unsigned int yWidth (y * width);
        const float& v (static_cast<float>(y * INV_IMG_HEIGHT));
        for (unsigned int x (0); x < width; x += 1)
        {
            // generate the ray
            const float& u (static_cast<float>(x * INV_IMG_WIDTH));
            this->camera_->getRay(u, v, ray);//constroi raio
            this->rTracer_->RayV(ray, rayRGB, isect);//faz trace do raio e escreve resultado em rayRGB

            // tonemap and convert to Paint
            canvas[x + yWidth] = ToneMapper::RGB2Color(rayRGB);
        }
    }
}

void Renderer::render(unsigned int *canvas,
                      const unsigned int numThreads) const//TODO: permitir lançar mais de 1 raio por pixel
{
    std::thread *threads (nullptr);
    if (numThreads > 1)
    {
        threads = new std::thread[numThreads - 1];
        for (unsigned int i (0); i < numThreads - 1; i++)
        {
            threads[i] = std::thread(&Renderer::thread_render, this, canvas, i, numThreads);
        }
    }
    thread_render(canvas, numThreads - 1, numThreads);
    for (unsigned int i (0); i < numThreads - 1; i++)
    {
        threads[i].join();
    }
    delete[] threads;
    delete this->scene_;
    delete this->camera_;
    delete this->rTracer_;
}