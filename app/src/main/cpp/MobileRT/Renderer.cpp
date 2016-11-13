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

void Renderer::call_from_thread(unsigned int *canvas, unsigned int startY, unsigned int stopY) const
{
    const float INV_IMG_WIDTH = 1.0f / this->width_;
    const float INV_IMG_HEIGHT = 1.0f / this->height_;
    Ray ray;
    RGB rayRGB;
    //#pragma omp parallel for num_threads(4)
    for (unsigned int y = startY; y < stopY; y++)
    {
        for (unsigned int x = 0; x < this->width_; x++)
        {
            // generate the ray
            const float u = static_cast<float>(x * INV_IMG_WIDTH);
            const float v = static_cast<float>(y * INV_IMG_HEIGHT);
            this->camera_->getRay(u, v, ray);//constroi raio
            this->rTracer_->RayV(ray, rayRGB);//faz trace do raio e escreve resultado em rayRGB

            // tonemap and convert to Paint
            canvas[x + y * this->width_] = ToneMapper::RGB2Color(rayRGB);
        }
    }
}

void Renderer::render(unsigned int *canvas,
                      const unsigned int numThreads) const//TODO: permitir lançar mais de 1 raio por pixel
{
    unsigned int verticalSection = this->height_ / numThreads;
    std::thread *threads = new std::thread[numThreads];
    for (unsigned int i = 0, h = 0; i < numThreads; i++, h += verticalSection) {
        threads[i] = std::thread(&Renderer::call_from_thread, this, canvas, h, h + verticalSection);
    }
    for (unsigned int i = 0; i < numThreads; i++) {
        threads[i].join();
    }
    delete threads;
    delete this->scene_;
    delete this->camera_;
    delete this->rTracer_;
}