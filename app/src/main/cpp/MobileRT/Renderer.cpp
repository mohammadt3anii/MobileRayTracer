//
// Created by Tiago on 16-10-2016.
//

#include "Renderer.h"
#include "Scenes/SceneCornell.h"
#include "Scenes/SceneSpheres.h"
#include "Color_Models/ToneMapper.h"
#include "Utils.h"
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
            this->camera_ = new PerspectiveCamera(Point3D(0.0f, 0.0f, -3.4f), hFov, vFov);
        }
            break;

        case 1 : // spheres
        {
            this->scene_ = new SceneSpheres();
            const float hFov = 60.0f;
            const float vFov = hFov * ratio;
            this->camera_ = new PerspectiveCamera(Point3D(0.0f, 0.5f, 1.0f), hFov, vFov);
        }
            break;

        default:
            break;
    }
    // create the ray tracer
    this->rayTracer_ = new RayTracer(*this->scene_, whichShader);
}

void Renderer::thread_render(unsigned int *canvas, unsigned int tid,
                                 unsigned int numThreads) const {
    const float width (this->width_);
    const float height (this->height_);
    const float INV_IMG_WIDTH (1.0f / width);
    const float INV_IMG_HEIGHT (1.0f / height);
    
    RGB rayRGB;
    Intersection isect;
    Vector3D vector;
    Ray ray;
    for (unsigned int y = tid; y < height; y += numThreads)
    {
        const unsigned int yWidth(y * this->width_);
        const float v (static_cast<float>(y * INV_IMG_HEIGHT));
        const float v_alpha (fastArcTan(-this->camera_->vFov_ * (v - 0.5f)));
        for (unsigned int x (0); x < width; x += 1)
        {
            // generate the ray
            const float u (static_cast<float>(x * INV_IMG_WIDTH));
            const float u_alpha (fastArcTan(this->camera_->hFov_ * (u - 0.5f)));
            this->camera_->getRay(ray, u_alpha, v_alpha);//constroi raio e coloca em ray
            //faz trace do raio e coloca a cor em rayRGB
            this->rayTracer_->rayTrace(rayRGB, ray, isect, vector);

            // tonemap and convert to Paint
            canvas[x + yWidth] = ToneMapper::RGB2Color(rayRGB);
        }
    }
}

void Renderer::render(unsigned int *canvas,
                      const unsigned int numThreads) const
{
    std::thread *threads = new std::thread[numThreads - 1];
    for (unsigned int i (0); i < numThreads - 1; i++)
    {
        threads[i] = std::thread(&Renderer::thread_render, this, canvas, i, numThreads);
    }
    thread_render(canvas, numThreads - 1, numThreads);
    for (unsigned int i (0); i < numThreads - 1; i++)
    {
        threads[i].join();
    }
    delete[] threads;
    delete this->scene_;
    delete this->camera_;
    delete this->rayTracer_;
}