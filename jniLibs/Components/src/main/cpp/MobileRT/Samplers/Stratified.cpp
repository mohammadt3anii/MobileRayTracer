//
// Created by Tiago on 21-11-2016.
//

#include "Stratified.h"

using namespace MobileRT;

Stratified::Stratified(const unsigned int domainSize, const unsigned int samples) :
        Sampler(domainSize, samples) {
}

float Stratified::getDeviation(const unsigned int num) {
    return this->deviation(num + 1);
}

float Stratified::getSample(const unsigned int tasks, const unsigned int sample) {
    unsigned int task(this->task_.fetch_add(tasks, std::memory_order_relaxed));
    return static_cast<float> (task - (sample * this->domainSize_)) / this->domainSize_;
}

/*void Stratified::renderScene(unsigned int *const bitmap,
                             const unsigned int,
                             const unsigned int) {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    RGB rayRGB;
    Intersection intersection;
    Vector3D vector;
    Ray ray;
    if (this->samples_ < 2)//1 sample per pixel
    {
        for (unsigned int y(getTasks(1, 0)); y < this->height_; y = getTasks(1, 0)) {
            const unsigned int yWidth(y * this->width_);
            const float v_alpha(fastArcTan(this->camera_.vFov_ * (0.5f - (y * INV_IMG_HEIGHT))));
            for (unsigned int x(0); x < this->width_; x++) {
                const float u_alpha(
                        fastArcTan(this->camera_.hFov_ * ((x * INV_IMG_WIDTH) - 0.5f)));
                //builds ray and puts it in ray variable
                this->camera_.getRay(ray, u_alpha, v_alpha);
                //ray trace and puts the color in rayRGB variable
                this->shader_.rayTrace(rayRGB, ray, intersection, vector);
                bitmap[x + yWidth] = rayRGB.RGB2Color();
            }
        }
    }
    else//multiple samples per pixel
    {
        RGB average;
        const float pixelHeight(0.5f / this->height_);
        const float pixelWidth(0.5f / this->width_);
        for (unsigned int i(0); i < this->samples_; i++) {
            const float deviationV(deviation(i + 1));
            for (unsigned int j(0); j < this->samples_; j++) {
                for (
                        unsigned int y(getTasks(1, i * this->samples_ + j));
                        y < this->height_;
                        y = getTasks(1, i * this->samples_ + j)) {
                    const unsigned int yWidth(y * this->width_);
                    const float v(y * INV_IMG_HEIGHT);
                    const float v_alpha(fastArcTan(this->camera_.vFov_ * (0.5f - v)));
                    for (unsigned int x(0); x < this->width_; x++) {
                        const float u(x * INV_IMG_WIDTH);
                        const float u_alpha(fastArcTan(this->camera_.hFov_ * (u - 0.5f)));
                        const float deviationU(deviation(j + 1));
                        const float u_alpha_deviation(u_alpha + (deviationU * pixelWidth));
                        const float v_alpha_deviation(v_alpha + (deviationV * pixelHeight));
                        //builds ray and puts it in ray variable
                        this->camera_.getRay(ray, u_alpha_deviation, v_alpha_deviation);
                        //ray trace and puts the color in rayRGB variable
                        this->shader_.rayTrace(rayRGB, ray, intersection, vector);
                        this->accumulate_[x + yWidth].addSample(average, rayRGB);
                        average.average();
                        // toneMap and convert to Paint
                        bitmap[x + yWidth] = average.RGB2Color();
                    }
                }
            }
        }
    }
}*/
