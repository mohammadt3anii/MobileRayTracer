//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.h"
#include "../Scenes/Cornell.h"
#include "../Scenes/Spheres.h"
#include "../Color_Models/ToneMapper.h"
#include "../Utils.h"
#include <thread>
#include <iostream>
#include <cstdlib>

using namespace MobileRT;

Sampler::Sampler(const unsigned int width, const unsigned int height,
                   const unsigned int whichScene, const unsigned int whichShader, const unsigned int samples) :
        width_(width),
        height_(height),
        samples_(samples)
{
    const float ratio = static_cast<float>(height_) / static_cast<float>(width_);

    // create and load the Scene, parameterize the camera
    switch (whichScene)
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
    this->rayTracer_ = new RayTracer(*this->scene_, whichShader);
}

Sampler::~Sampler ()
{
    delete this->scene_;
    delete this->camera_;
    delete this->rayTracer_;
}

float Sampler::deviation (const int index) const
{
    const float start (-0.5f);
    const float end (0.5f);
    const float inc ((end - start) / this->samples_);
    return (start + (index*inc) - (inc/2.0f));
}