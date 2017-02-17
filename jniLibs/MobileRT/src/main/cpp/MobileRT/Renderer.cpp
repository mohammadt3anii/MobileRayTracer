//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.h"

using namespace MobileRT;

Renderer::Renderer(Sampler &sampler, const Shader &shader, const Camera &camera,
                   const unsigned int width, const unsigned int height) :
        sampler_(sampler),
        shader_(shader),
        camera_(camera),
        width_(roundToEvenNumber(width)),
        height_(roundToEvenNumber(height)),
        accumulate_(new RGB[roundToEvenNumber(width) * roundToEvenNumber(height)]),
        domainSize_(roundToEvenNumber(width) * roundToEvenNumber(height)),
        blockSize_(1) {
}

Renderer::~Renderer() {
    delete[] this->accumulate_;
}

void Renderer::renderFrame(unsigned int *const bitmap, const unsigned int numThreads) {
    const unsigned int size(this->width_ * this->height_);
    for (unsigned int i(0); i < size; i++) {
        this->accumulate_[i].reset();
    }
    this->sampler_.resetTask();
    std::thread *const threads(new std::thread[numThreads - 1]);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i] = std::thread(&Renderer::renderScene, this, bitmap, i, numThreads);
    }
    this->renderScene(bitmap, numThreads - 1, numThreads);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i].join();
    }
    delete[] threads;
}

void Renderer::stopRender() {
    this->height_ = 0;
    this->width_ = 1;
    this->domainSize_ = 0;
    this->blockSize_ = 0;
    this->sampler_.stopRender();
}

void Renderer::renderScene(unsigned int *const bitmap,
                           const unsigned int,
                           const unsigned int) {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelWidth(0.5f / this->width_);
    const float pixelHeight(0.5f / this->height_);
    blockSize_ = 1;
    RGB rayRGB;
    RGB average;
    Intersection intersection;
    Vector3D vector;
    Ray ray;

    for (unsigned int i(0); i < this->sampler_.samples_; i++)//change u
    {
        const float deviationV(this->sampler_.getDeviation(i + 1));
        for (unsigned int j(0); j < this->sampler_.samples_; j++)//change v
        {
            const float deviationU(this->sampler_.getDeviation(j + 1));
            for (unsigned int task(0); task < this->domainSize_; task++) {//change y
                float block(
                        this->sampler_.getTask(this->blockSize_, i * this->sampler_.samples_ + j));
                const unsigned int pixel(
                        static_cast<unsigned int> (block * this->domainSize_ + 0.5f));
                const unsigned line(pixel / width_);
                const float v(line * INV_IMG_HEIGHT);
                const float v_alpha(fastArcTan(this->camera_.vFov_ * (0.5f - v)));
                const unsigned int startX(pixel % width_);
                for (unsigned int x(0); x < this->blockSize_; x++)//change x
                {
                    const unsigned column(startX + x);
                    //LOG("p=%u,x=%u,y=%u", pixel, column, line);
                    const float u(column * INV_IMG_WIDTH);
                    const float u_alpha(fastArcTan(this->camera_.hFov_ * (u - 0.5f)));
                    const float u_alpha_deviation(u_alpha + (deviationU * pixelWidth));
                    const float v_alpha_deviation(v_alpha + (deviationV * pixelHeight));
                    this->camera_.getRay(ray, u_alpha_deviation, v_alpha_deviation);
                    this->shader_.rayTrace(rayRGB, ray, intersection, vector);
                    this->accumulate_[pixel + x].addSample(average, rayRGB);
                    average.average();
                    bitmap[pixel + x] = average.RGB2Color();
                }
            }
        }
    }
}
