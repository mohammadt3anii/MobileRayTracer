//
// Created by Tiago on 21-11-2016.
//

#include "Jittered.h"

using namespace MobileRT;

Jittered::Jittered(const unsigned int domainSize, const unsigned int samples) :
        Sampler(domainSize, samples),
        half_rand_max_(RAND_MAX * 0.5f) {
}

float Jittered::getDeviation(const unsigned) {
    return std::rand() / half_rand_max_ - 1.0f;
}

float Jittered::getTask(const unsigned int tasks, const unsigned int) {
    unsigned int task = this->task_.fetch_add(tasks, std::memory_order_relaxed);
    return haltonSequence(task, 2);
}

/*void Jittered::renderScene(unsigned int *const bitmap,
                           const unsigned int,
                           const unsigned int) {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelWidth(0.5f / this->width_);
    const float pixelHeight(0.5f / this->height_);
    const float half_rand_max(RAND_MAX * 0.5f);
    RGB rayRGB;
    RGB average;
    Intersection intersection;
    Vector3D vector;
    Ray ray;

    const unsigned int requestTasks(1);
    for (
            unsigned int taskId(getTasks(requestTasks, 0));
            taskId < this->maxHalton_;
            taskId = getTasks(requestTasks, 0))
    {
        const unsigned taskMax(taskId + requestTasks);
        for (unsigned int j(taskId); j < taskMax && j < this->maxHalton_; j++) {
            const double halton(haltonSequence(j, 2));
            const unsigned int i(static_cast<unsigned int>(round(halton * this->maxHalton_)));
            if (i >= this->tasks_) continue;
            const unsigned int ii(i / this->samples_);

            RGB &accumulate(this->accumulate_[ii]);
            if (accumulate.samples_ == this->samples_) {
                continue;
            }
            const unsigned int x(ii % this->width_);
            const unsigned int y(ii / this->width_);

            // generate the ray
            const float u_alpha(fastArcTan(this->camera_.hFov_ * ((x * INV_IMG_WIDTH) - 0.5f)));
            const float v_alpha(fastArcTan(this->camera_.vFov_ * (0.5f - (y * INV_IMG_HEIGHT))));
            const float randU(std::rand() / half_rand_max - 1.0f);
            const float randV(std::rand() / half_rand_max - 1.0f);
            const float u_alpha_jittered(u_alpha + (randU * pixelWidth));
            const float v_alpha_jittered(v_alpha + (randV * pixelHeight));
            //builds ray and puts it in ray variable
            this->camera_.getRay(ray, u_alpha_jittered, v_alpha_jittered);
            //ray trace and puts the color in rayRGB variable
            this->shader_.rayTrace(rayRGB, ray, intersection, vector);
            accumulate.addSample(average, rayRGB);
            average.average();
            // toneMap and convert to Paint
            bitmap[ii] = average.RGB2Color();
        }
    }
}*/
