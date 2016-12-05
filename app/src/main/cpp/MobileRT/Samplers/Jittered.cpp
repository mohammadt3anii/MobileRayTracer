//
// Created by Tiago on 21-11-2016.
//

#include "Jittered.h"
#include "../Utils.h"
#include "../Color_Models/ToneMapper.h"
#include <cstdlib>

using namespace MobileRT;

Jittered::Jittered(const unsigned int width, const unsigned int height,
                   const unsigned int scene, const unsigned int shader,
                   const unsigned int samples) :
        Sampler(width, height, scene, shader, samples)
{
}

void Jittered::renderScene(unsigned int *canvas, unsigned int tid,
                           unsigned int numThreads) {
    const float width (this->width_);
    const float height (this->height_);
    const float INV_IMG_WIDTH (1.0f / width);
    const float INV_IMG_HEIGHT (1.0f / height);
    const float pixelHeight (0.5f/this->height_);
    const float pixelWidth (0.5f/this->width_);
    const float half_rand_max(RAND_MAX * 0.5f);
    RGB rayRGB;
    RGB average;
    Intersection intersection;
    Vector3D vector;
    Ray ray;
    for (unsigned int i(0); i < this->samples_; i++)
    {
        //for (unsigned int y(getTask(i)); y < this->height_; y = getTask(i))
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        for (unsigned int y(static_cast<unsigned int> (haltonSequence(getTask(i), 2) *
                                                       this->height_)); ; y = static_cast<unsigned int> (
                haltonSequence(getTask(i), 2) * this->height_))
        {
            const unsigned int yWidth(y * this->width_);
            const float v(y * INV_IMG_HEIGHT);
            const float v_alpha(fastArcTan(this->camera_->vFov_ * (0.5f - v)));
            for (unsigned int x(0); x < this->width_; x += 1)
            {
                // generate the ray
                const float u(x * INV_IMG_WIDTH);
                const float u_alpha(fastArcTan(this->camera_->hFov_ * (u - 0.5f)));
                const float randU(std::rand() / half_rand_max - 1.0f);
                const float randV(std::rand() / half_rand_max - 1.0f);
                const float u_alpha_jittered(u_alpha + (randU * pixelWidth));
                const float v_alpha_jittered(v_alpha + (randV * pixelHeight));
                //builds ray and puts it in ray variable
                this->camera_->getRay(ray, u_alpha_jittered, v_alpha_jittered);
                //ray trace and puts the color in rayRGB variable
                this->rayTracer_->rayTrace(rayRGB, ray, intersection, vector);
                accumulate_[x + yWidth].addSample(average, rayRGB);
                average.average();
                // toneMap and convert to Paint
                canvas[x + yWidth] = ToneMapper::RGB2Color(average);
            }
        }
    }
}