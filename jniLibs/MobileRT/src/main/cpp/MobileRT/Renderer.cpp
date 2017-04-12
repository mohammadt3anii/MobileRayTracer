//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.h"

using namespace MobileRT;

Renderer::Renderer(Sampler &samplerCamera, Shader &shader, const Camera &camera,
                   const unsigned int width, const unsigned int height,
                   const unsigned int blockSizeX, const unsigned int blockSizeY,
                   Sampler &samplerPixel) :
        samplerCamera_(samplerCamera),
        shader_(shader),
        camera_(camera),
        width_(roundDownEvenNumber(width)),
        height_(roundDownEvenNumber(height)),
        accumulate_(new RGB[roundDownEvenNumber(width) * roundDownEvenNumber(height)]),
        domainSize_(
                (roundDownEvenNumber(width) /
                    roundDownMultipleOf(blockSizeX, roundDownEvenNumber(width))) *
                (roundDownEvenNumber(height) /
                        roundDownMultipleOf(blockSizeY, roundDownEvenNumber(height)))),
        blockSizeX_(roundDownMultipleOf(blockSizeX, roundDownEvenNumber(width))),
        blockSizeY_(roundDownMultipleOf(blockSizeY, roundDownEvenNumber(height))),
        resolution_(roundDownEvenNumber(width) * roundDownEvenNumber(height)),
        imagePlane_(new RGB[roundDownEvenNumber(width) * roundDownEvenNumber(height)]),
        max_(1.0f),
        samplerPixel_(samplerPixel)
{
}

Renderer::~Renderer(void) {
    delete[] this->accumulate_;
    delete[] this->imagePlane_;
}

void Renderer::renderFrame(unsigned int *const bitmap,
const unsigned int numThreads) {
    const unsigned int size(this->width_ * this->height_);
    for (unsigned int i(0); i < size; i++) {
        this->accumulate_[i].reset();
    }
    this->samplerCamera_.resetSampling();
    this->samplerPixel_.resetSampling();
    this->shader_.resetSampling();
    std::thread *const threads(new std::thread[numThreads - 1]);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i] = std::thread(&Renderer::renderScene, this, bitmap, i);
    }
    renderScene(bitmap, numThreads - 1);
    for (unsigned int i(0); i < numThreads - 1; i++) {
        threads[i].join();
    }
    delete[] threads;

    float max(-1.0f);
    for (unsigned int i (0); i < this->height_; i++)
    {
        for (unsigned int j (0); j < this->width_; j++)
        {
            const unsigned int pixel (i*this->width_ + j);
            const float pixelMax (this->imagePlane_[pixel].getMax());
            max = pixelMax > this->max_? pixelMax : this->max_;
        }
    }
    LOG("max = %f", double(max));
    this->max_ = max;
    if (max > 0.0f && max != 1.0f && std::fabs(max - this->max_) > 0.0001f)
    {
        for (unsigned int i (0); i < this->height_; i++)
        {
            for (unsigned int j (0); j < this->width_; j++)
            {
                const unsigned int pixel (i*this->width_ + j);
                RGB& aux = this->imagePlane_[pixel];
                aux /= this->max_;
                bitmap[pixel] = aux.RGB2Color();
            }
        }
    }

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
    this->samplerCamera_.stopRender();
    this->samplerPixel_.stopRender();
}

void Renderer::renderScene(unsigned int *const bitmap, const unsigned int) {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelWidth(0.5f / this->width_);
    const float pixelHeight(0.5f / this->height_);
    const unsigned int samples(static_cast<unsigned int> (this->samplerCamera_.samples_));
    Intersection intersection;
    Ray ray;

    for (unsigned int sample(0); sample < samples; sample++)
    {
        for (float block(this->samplerCamera_.getSample(sample));;
                block = this->samplerCamera_.getSample(sample))
        {
            if (block >= 1.0f) break;
            const unsigned int pixel(
                static_cast<unsigned int> (block * this->domainSize_ + 0.5f) *
                this->blockSizeX_ % resolution_);
            const unsigned int startY(((pixel / width_) * blockSizeY_) % height_);
            const unsigned int endY(startY + this->blockSizeY_);
            //LOG("tid = %u, block = %f, sample = %u, Y = [%u, %u]",
            //    tid, double(block), sample, startY, endY);
            for (unsigned int y(startY); y < endY; y++) {
                const unsigned int yWidth(y * width_);
                const float v(y * INV_IMG_HEIGHT);
                const float v_alpha(fastArcTan(this->camera_.vFov_ * (0.5f - v)));
                const unsigned int startX((pixel + yWidth) % width_);
                const unsigned int endX(startX + this->blockSizeX_);
                for (unsigned int x(startX); x < endX; x++) {
                    const float u(x * INV_IMG_WIDTH);
                    const float deviationU((this->samplerPixel_.getSample(sample) - 0.5f) * 2.0f);
                    const float deviationV((this->samplerPixel_.getSample(sample) - 0.5f) * 2.0f);
                    const float u_alpha(fastArcTan(this->camera_.hFov_ * (u - 0.5f)));
                    const float u_alpha_deviation(u_alpha + (deviationU * pixelWidth));
                    const float v_alpha_deviation(v_alpha + (deviationV * pixelHeight));
                    this->camera_.getRay(ray, u_alpha_deviation, v_alpha_deviation);
                    RGB &aux = this->imagePlane_[yWidth + x];
                    this->shader_.rayTrace(aux, ray, intersection);
                    this->accumulate_[yWidth + x].addSampleAndCalcAvg(aux);
                    aux /= this->max_;
                    bitmap[yWidth + x] = aux.RGB2Color();
                }
            }
        }
    }
}
