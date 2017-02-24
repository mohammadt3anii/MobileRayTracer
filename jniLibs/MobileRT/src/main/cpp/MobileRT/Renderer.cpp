//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.h"

using namespace MobileRT;

Renderer::Renderer(Sampler &sampler, Shader &shader, const Camera &camera,
                   const unsigned int width, const unsigned int height,
                   const unsigned int blockSizeX, const unsigned int blockSizeY) :
        sampler_(sampler),
        shader_(shader),
        camera_(camera),
        width_(roundToEvenNumber(width)),
        height_(roundToEvenNumber(height)),
        accumulate_(new RGB[roundToEvenNumber(width) * roundToEvenNumber(height)]),
        domainSize_(
                (roundToEvenNumber(width) /
                 roundToMultipleOf(blockSizeX, roundToEvenNumber(width))) *
                (roundToEvenNumber(height) /
                 roundToMultipleOf(blockSizeY, roundToEvenNumber(height)))),
        blockSizeX_(roundToMultipleOf(blockSizeX, roundToEvenNumber(width))),
        blockSizeY_(roundToMultipleOf(blockSizeY, roundToEvenNumber(height))),
        resolution_(roundToEvenNumber(width) * roundToEvenNumber(height)) {
}

Renderer::~Renderer(void) {
    delete[] this->accumulate_;
}

void Renderer::renderFrame(unsigned int *const bitmap, const unsigned int numThreads) {
    const unsigned int size(this->width_ * this->height_);
    for (unsigned int i(0); i < size; i++) {
        this->accumulate_[i].reset();
    }
    this->sampler_.resetSampling();
    std::thread *const threads(new std::thread[numThreads - 1]);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i] = std::thread(&Renderer::renderScene, this, bitmap, i, numThreads);
    }
    this->renderScene(bitmap, numThreads - 1, numThreads);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i].join();
    }
    delete[] threads;

    /*LOG("%s%u", "START - resolution=", resolution_);
    LOG("point3D = %u", Point3D::getInstances());
    LOG("vector3D = %u", Vector3D::getInstances());
    LOG("RGB = %u", RGB::getInstances());
    LOG("scene = %u", Scene::getInstances());
    LOG("ray = %u", Ray::getInstances());
    LOG("material = %u", Material::getInstances());
    LOG("intersection = %u", Intersection::getInstances());
    LOG("%s", "FINISH");*/
}

void Renderer::stopRender(void) {
    this->blockSizeX_ = 0;
    this->blockSizeY_ = 0;
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
    RGB rgb;
    Intersection intersection;
    Ray ray;

    for (unsigned int i(0); i < samples; i++)
    {
        for (unsigned int j(0); j < samples; j++)
        {
            const unsigned int sample(i * samples + j);
            for (float block(this->sampler_.getSample(1, sample));
                 this->sampler_.notFinished(sample); block = this->sampler_.getSample(1, sample)) {
                const unsigned int pixel(
                        static_cast<unsigned int> (block * this->domainSize_ + 0.5f) *
                        this->blockSizeX_ % resolution_);
                const unsigned int startY(((pixel / width_) * blockSizeY_) % height_);
                for (unsigned int y(startY); y < startY + this->blockSizeY_; y++) {
                    const unsigned int yWidth(y * width_);
                    const float v(y * INV_IMG_HEIGHT);
                    const float v_alpha(fastArcTan(this->camera_.vFov_ * (0.5f - v)));
                    const unsigned int startX((pixel + yWidth) % width_);
                    for (unsigned int x(startX); x < startX + this->blockSizeX_; x++) {
                        const float u(x * INV_IMG_WIDTH);
                        const float deviationU(this->sampler_.getDeviation(j + 1));
                        const float deviationV(this->sampler_.getDeviation(i + 1));
                        const float u_alpha(fastArcTan(this->camera_.hFov_ * (u - 0.5f)));
                        const float u_alpha_deviation(u_alpha + (deviationU * pixelWidth));
                        const float v_alpha_deviation(v_alpha + (deviationV * pixelHeight));
                        this->camera_.getRay(ray, u_alpha_deviation, v_alpha_deviation);
                        //Ray ray(u_alpha_deviation, v_alpha_deviation, 1.0f, camera_.position_);
                        this->shader_.rayTrace(rgb, ray, intersection);
                        /*if (yWidth + x >= resolution_) {
                            LOG("x=%u[%u,%u],y=%u[%u,%u],pixel=%u[%u,%u],block=%f,p=%u,i=%u,j=%u",
                                x, startX, startX + blockSizeX_,
                                y, startY, startY + blockSizeY_,
                                yWidth + x, resolution_, domainSize_,
                                double(block), pixel, i, j);
                        }*/
                        this->accumulate_[yWidth + x].addSampleAndCalcAvg(rgb);
                        bitmap[yWidth + x] = rgb.RGB2Color();
                    }
                }
            }
        }
    }
}
