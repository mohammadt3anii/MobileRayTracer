//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.h"

using namespace MobileRT;

Renderer::Renderer(Sampler &sampler, Shader &shader, const Camera &camera,
                   const unsigned int width, const unsigned int height,
                   const unsigned int blockSize) :
        sampler_(sampler),
        shader_(shader),
        camera_(camera),
        width_(roundToEvenNumber(width)),
        height_(roundToEvenNumber(height)),
        accumulate_(new RGB[roundToEvenNumber(width) * roundToEvenNumber(height)]),
        domainSize_((roundToEvenNumber(width)) * (roundToEvenNumber(height))),
        blockSize_(blockSize),
        resolution_(roundToEvenNumber(width) * roundToEvenNumber(height)) {
}

Renderer::~Renderer(void) {
    delete[] this->accumulate_;
}

void Renderer::renderFrame(unsigned int *const bitmap, const unsigned int numThreads) {
    this->shader_.setBitmap(bitmap, this->accumulate_, this->camera_.hFov_, this->camera_.vFov_,
                            width_, height_);
    const unsigned int size(this->width_ * this->height_);
    this->sampler_.resetTask(size);
    for (unsigned int i(0); i < size; i++) {
        this->accumulate_[i].reset();
    }
    this->sampler_.resetTask(0);
    std::thread *const threads(new std::thread[numThreads - 1]);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i] = std::thread(&Renderer::renderScene, this, bitmap, i, numThreads);
    }
    this->renderScene(bitmap, numThreads - 1, numThreads);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i].join();
    }
    delete[] threads;

    LOG("%s%u", "START - resolution=", resolution_);
    LOG("point3D = %u", Point3D::getInstances());
    LOG("vector3D = %u", Vector3D::getInstances());
    LOG("RGB = %u", RGB::getInstances());
    LOG("scene = %u", Scene::getInstances());
    LOG("ray = %u", Ray::getInstances());
    LOG("material = %u", Material::getInstances());
    LOG("intersection = %u", Intersection::getInstances());
    LOG("%s", "FINISH");
}

void Renderer::stopRender(void) {
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
    const unsigned int samples(this->sampler_.samples_);
    RGB rayRGB;
    RGB average;
    Intersection intersection;
    Ray ray;
    float block(0.0f);

    for (unsigned int i(0); i < samples; i++)//change u
    {
        const float deviationV(this->sampler_.getDeviation(i + 1));
        for (unsigned int j(0); j < samples; j++)//change v
        {
            const unsigned int sample(i * samples + j);
            //LOG("i=%u, j=%u, sample=%u", i, j, sample);
            const float deviationU(this->sampler_.getDeviation(j + 1));
            //for (unsigned int task(0); this->sampler_.notFinished(sample); task++) {//change y
            do {
                block = this->sampler_.getSample(1, sample);
                const unsigned int pixel(
                        static_cast<unsigned int> (block * this->domainSize_ + 0.5f));
                LOG("p=%u[%u]", pixel, domainSize_);
                for (unsigned int y(0); y < this->blockSize_; y++)//change x
                {
                    unsigned line((pixel + y) / width_);
                    //line *= this->blockSize_;
                    LOG("line=%u[%u]", line, height_);
                    const float v(line * INV_IMG_HEIGHT);
                    const float v_alpha(fastArcTan(this->camera_.vFov_ * (0.5f - v)));
                    const unsigned int startX((pixel + y) % width_);
                    for (unsigned int x(0); x < this->blockSize_; x++)//change x
                    {
                        const unsigned column(startX + x);
                        //LOG("p=%u,x=%u,y=%u", pixel, column, line);
                        const float u(column * INV_IMG_WIDTH);
                        //u_alpha = arctg (hFov_ * x/width - 0.5)
                        //hFov_ * x/width - 0.5 = tg (u_alpha)
                        //hFov_ * x/width = tg (u_alpha) + 0.5
                        //x = (tg (u_alpha) + 0.5) * width / hFov_
                        //y = (tg (v_alpha) / vFov_ + 0.5) * height
                        //v_alpha = arctg (vFov_ * (0.5 - y/height))
                        //vFov_ * (0.5f - y/height) = tg (v_alpha)
                        //0.5 - y/height = tg (v_alpha) / vFov_
                        const float u_alpha(fastArcTan(this->camera_.hFov_ * (u - 0.5f)));
                        const float u_alpha_deviation(u_alpha + (deviationU * pixelWidth));
                        const float v_alpha_deviation(v_alpha + (deviationV * pixelHeight));
                        this->camera_.getRay(ray, u_alpha_deviation, v_alpha_deviation);
                        //Ray ray(u_alpha_deviation, v_alpha_deviation, 1.0f, this->camera_.position_);
                        this->shader_.rayTrace(rayRGB, ray, intersection);
                        this->accumulate_[pixel + x].addSample(average, rayRGB);
                        bitmap[pixel + x] = average.RGB2Color();
                    }
                }
            } while (this->sampler_.notFinished(sample));
        }
    }
}
