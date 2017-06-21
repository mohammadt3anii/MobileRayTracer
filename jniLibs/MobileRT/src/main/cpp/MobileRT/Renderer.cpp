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
        width_(width),
        height_(height),
        accumulate_(new RGB[width * height]),
        domainSize_((width / blockSizeX) * (height / blockSizeY)),
        blockSizeX_(blockSizeX),
        blockSizeY_(blockSizeY),
        resolution_(width * height),
        imagePlane_(new RGB[width * height]),
        max_(1.0f),
        samplerPixel_(samplerPixel),
        sample_(0u),
        toneMapper_([&](const float value){return value;})
{
}

Renderer::~Renderer(void) {
    delete[] this->accumulate_;
    delete[] this->imagePlane_;
}

void Renderer::renderFrame(unsigned int *const bitmap, const unsigned int numThreads) {
    this->sample_ = 0;
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
    unsigned int ii(0u);
    unsigned int jj(0u);
    for (unsigned int i (0); i < this->height_; i++)
    {
        for (unsigned int j (0); j < this->width_; j++)
        {
            const unsigned int pixel (i*this->width_ + j);
            const float pixelMax (this->imagePlane_[pixel].getMax());
            if (pixelMax > max) {
                max = pixelMax;
                ii = i;
                jj = j;
            }
        }
    }
    const unsigned int pixel2 (ii*this->width_ + jj);
    RGB& pixelRGB2 (this->imagePlane_[pixel2]);
    LOG("max = %f, i = %u, j = %u", double(max), ii, jj);
    LOG("rgb = %f %f %f", double(pixelRGB2.R_), double(pixelRGB2.G_), double(pixelRGB2.B_));

    this->max_ = max;
    ////if (max > 0.0f && max != 1.0f && std::fabs(max - this->max_) > 0.0001f)
    //{
    //    for (unsigned int i (0); i < this->height_; i++)
    //    {
    //        for (unsigned int j (0); j < this->width_; j++)
    //        {
    //            const unsigned int pixel (i*this->width_ + j);
    //            RGB& pixelRGB (this->imagePlane_[pixel]);
    //            //pixelRGB /= this->max_;
    //            pixelRGB.R_ = 1.0f - std::cos(std::sqrt(std::sqrt(pixelRGB.R_)));
    //            pixelRGB.G_ = 1.0f - std::cos(std::sqrt(std::sqrt(pixelRGB.G_)));
    //            pixelRGB.B_ = 1.0f - std::cos(std::sqrt(std::sqrt(pixelRGB.B_)));
    //            bitmap[pixel] = pixelRGB.RGB2Color();
    //        }
    //    }
    //}

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

void Renderer::registerToneMapper(std::function<float(const float)> toneMapper)
{
    toneMapper_ = toneMapper;
}

void Renderer::stopRender(void) {
    this->blockSizeX_ = 0;
    this->blockSizeY_ = 0;
    this->samplerCamera_.stopSampling();
    this->samplerPixel_.stopSampling();
}

void Renderer::toneMapper(RGB& pixel) {
    pixel.R_ = toneMapper_(pixel.R_);
    pixel.G_ = toneMapper_(pixel.G_);
    pixel.B_ = toneMapper_(pixel.B_);
}

void Renderer::renderScene(unsigned int *const bitmap, const unsigned int tid) {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelWidth(0.5f / this->width_);
    const float pixelHeight(0.5f / this->height_);
    const unsigned int samples(static_cast<unsigned int> (this->samplerCamera_.samples_));
    Intersection intersection;

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
            //LOG("tid=%u, block=%f, s=%u, Y=[%u, %u[", tid, double(block), sample, startY, endY);
            for (unsigned int y(startY); y < endY; y++) {
                const unsigned int yWidth(y * width_);
                const float v(y * INV_IMG_HEIGHT);
                const unsigned int startX((pixel + yWidth) % width_);
                const unsigned int endX(startX + this->blockSizeX_);
                for (unsigned int x(startX); x < endX; x++) {
                    const float u(x * INV_IMG_WIDTH);
                    const float r1 (this->samplerPixel_.getSample(0));
                    const float r2 (this->samplerPixel_.getSample(0));
                    const float deviationU((r1 - 0.5f) * 2.0f * pixelWidth);
                    const float deviationV((r2 - 0.5f) * 2.0f * pixelHeight);
                    Ray ray(this->camera_.generateRay(u, v, deviationU, deviationV));
                    RGB &pixelRGB (this->imagePlane_[yWidth + x]);
                    this->shader_.rayTrace(pixelRGB, ray, intersection);
                    this->accumulate_[yWidth + x].addSampleAndCalcAvg(pixelRGB);
                    toneMapper(pixelRGB);
                    bitmap[yWidth + x] = pixelRGB.RGB2Color();
                }
            }
        }
        if (tid == 0) this->sample_ = sample + 1;
    }
}

unsigned int Renderer::getSample(void) {
    return this->sample_;
}
