//
// Created by Tiago on 21-11-2016.
//

#include "Stratified.h"
#include "../Utils.h"
#include "../Color_Models/ToneMapper.h"
#include <cmath>

using namespace MobileRT;

Stratified::Stratified(const unsigned int width, const unsigned int height,
            const unsigned int whichScene, const unsigned int whichShader, const unsigned int samples) :
    Sampler(width, height, whichScene, whichShader, std::sqrt(samples)) {
}

void Stratified::renderScene(unsigned int *canvas, unsigned int tid,
                             unsigned int numThreads) {
    const float width (this->width_);
    const float height (this->height_);
    const float INV_IMG_WIDTH (1.0f / width);
    const float INV_IMG_HEIGHT (1.0f / height);
    RGB rayRGB;
    Intersection intersection;
    Vector3D vector;
    Ray ray;
    if (this->samples_ < 2)//1 sample per pixel
    {
        for (unsigned int y(getTask(0)); y < this->height_; y = getTask(0))
        {
            const unsigned int yWidth(y * this->width_);
            const float v(y * INV_IMG_HEIGHT);
            const float v_alpha (fastArcTan(this->camera_->vFov_ * (0.5f - v)));
            for (unsigned int x(0); x < this->width_; x += 1)
            {
                const float u(x * INV_IMG_WIDTH);
                const float u_alpha (fastArcTan(this->camera_->hFov_ * (u - 0.5f)));
                for(unsigned int i (0); i < this->samples_; i++)
                {
                    for(unsigned int j (0); j < this->samples_; j++)
                    {
                        //builds ray and puts it in ray variable
                        this->camera_->getRay(ray, u_alpha, v_alpha);
                        //ray trace and puts the color in rayRGB variable
                        this->rayTracer_->rayTrace(rayRGB, ray, intersection, vector);
                    }
                }
                canvas[x + yWidth] = ToneMapper::RGB2Color(rayRGB);
            }
        }
    }
    else//multiple samples per pixel
    {
        RGB average;
        const float pixelHeight(0.5f / this->height_);
        const float pixelWidth(0.5f / this->width_);
        for (unsigned int i(1); i <= this->samples_; i++)
        {
            const float deviationV(deviation(i));
            for (unsigned int j(1); j <= this->samples_; j++)
            {
                for (unsigned int y(getTask((i - 1) * this->samples_ + (j - 1)));
                     y < this->height_; y = getTask((i - 1) * this->samples_ + (j - 1)))
                {
                    const unsigned int yWidth(y * this->width_);
                    const float v(y * INV_IMG_HEIGHT);
                    const float v_alpha(fastArcTan(this->camera_->vFov_ * (0.5f - v)));
                    for (unsigned int x(0); x < this->width_; x += 1)
                    {
                        const float u(x * INV_IMG_WIDTH);
                        const float u_alpha(fastArcTan(this->camera_->hFov_ * (u - 0.5f)));
                        const float deviationU(deviation(j));
                        const float u_alpha_deviation(u_alpha + (deviationU * pixelWidth));
                        const float v_alpha_deviation(v_alpha + (deviationV * pixelHeight));
                        //builds ray and puts it in ray variable
                        this->camera_->getRay(ray, u_alpha_deviation, v_alpha_deviation);
                        //ray trace and puts the color in rayRGB variable
                        this->rayTracer_->rayTrace(rayRGB, ray, intersection, vector);
                        this->accumulate_[x + yWidth].addSample(average, rayRGB);
                        average.average();
                        // toneMap and convert to Paint
                        canvas[x + yWidth] = ToneMapper::RGB2Color(average);
                    }
                }
            }
        }
    }
}