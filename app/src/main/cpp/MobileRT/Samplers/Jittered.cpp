//
// Created by Tiago on 21-11-2016.
//

#include "Jittered.h"
#include "../Utils.h"
#include "../Color_Models/ToneMapper.h"
#include <cstdlib>
//#include <random>

using namespace MobileRT;

Jittered::Jittered(
        const unsigned int width, const unsigned int height,
        const unsigned int shader, const unsigned int samples,
        const Perspective &camera, const Scene &scene) :
        Sampler(width, height, shader, samples, camera, scene)
{
}

bool Jittered::finishSampling(unsigned int &line) const {
    for (; line < this->height_; line++) {
        if (this->accumulate_[line * this->width_].samples_ < this->samples_) {
            return false;
        }
    }
    return true;
}

void Jittered::renderScene(unsigned int *canvas) {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelHeight (0.5f/this->height_);
    const float pixelWidth (0.5f/this->width_);
    const float half_rand_max(RAND_MAX * 0.5f);
    //const float half_rand_max((std::pow(2,96)-1) * 0.5f);
    unsigned int line(0);
    RGB rayRGB;
    RGB average;
    Intersection intersection;
    Vector3D vector;
    Ray ray;
    //for (unsigned int y(getTask(i)); y < this->height_; y = getTask(i))
    for (unsigned int y(static_cast<unsigned int> (haltonSequence(getTask(0), 2) * this->height_));
         y < this->height_;
         y = static_cast<unsigned int> (haltonSequence(getTask(0), 2) * this->height_))
    {
        const unsigned int yWidth(y * this->width_);
        const float v_alpha(fastArcTan(this->camera_->vFov_ * (0.5f - (y * INV_IMG_HEIGHT))));
        for (unsigned int x(0); x < this->width_; x++)
        {
            // generate the ray
            const float u_alpha(fastArcTan(this->camera_->hFov_ * ((x * INV_IMG_WIDTH) - 0.5f)));
            const float randU(std::rand() / half_rand_max - 1.0f);
            const float randV(std::rand() / half_rand_max - 1.0f);
            const float u_alpha_jittered(u_alpha + (randU * pixelWidth));
            const float v_alpha_jittered(v_alpha + (randV * pixelHeight));
            //builds ray and puts it in ray variable
            this->camera_->getRay(ray, u_alpha_jittered, v_alpha_jittered);
            //ray trace and puts the color in rayRGB variable
            this->rayTracer_->rayTrace(rayRGB, ray, intersection, vector);
            this->accumulate_[x + yWidth].addSample(average, rayRGB);
            average.average();
            // toneMap and convert to Paint
            canvas[x + yWidth] = ToneMapper::RGB2Color(average);
        }
        if (this->accumulate_[yWidth].samples_ == this->samples_ && finishSampling(line)) {
            stopSampling();
            return;
        }
    }
}