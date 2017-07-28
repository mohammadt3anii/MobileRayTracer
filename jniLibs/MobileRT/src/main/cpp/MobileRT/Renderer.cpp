//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.hpp"

using MobileRT::Renderer;

Renderer::Renderer(Sampler *samplerCamera, Shader *shader, Camera const &camera,
                   const unsigned int width, const unsigned int height,
                   const unsigned int blockSizeX, const unsigned int blockSizeY,
                   Sampler *samplerPixel) noexcept :
        samplerCamera_(samplerCamera),
        samplerPixel_(samplerPixel),
        camera_(camera),
        shader_(shader),
        accumulate_(std::vector<RGB>(width * height)),
        domainSize_((width / blockSizeX) * (height / blockSizeY)),
        width_(width),
        height_(height),
        blockSizeX_(blockSizeX),
        blockSizeY_(blockSizeY),
        resolution_(width * height),
        sample_(0u)
{
}

void Renderer::renderFrame(unsigned int *const bitmap, const unsigned int numThreads) noexcept {
	LOG("START - resolution = ", resolution_);
	this->sample_ = 0u;
	const unsigned int size(this->width_ * this->height_);
	for (unsigned int i(0u); i < size; i++) {
			this->accumulate_[i].reset();
	}
	this->samplerCamera_->resetSampling();
	this->samplerPixel_->resetSampling();
	this->shader_->resetSampling();
	const unsigned numChildren (numThreads - 1u);
	std::vector<std::thread> threads;
	threads.reserve(numChildren);
	for (unsigned int i(0u); i < numChildren; i++) {
		threads.emplace_back(&Renderer::renderScene, this, bitmap, i);
	}
	renderScene(bitmap, numChildren);
	for (std::thread& thread : threads) {
		thread.join();
	}
	threads.clear();

	float max(-1.0f);
	unsigned int ii(0u);
	unsigned int jj(0u);
	for (unsigned int i (0); i < this->height_; i++) {
		for (unsigned int j (0); j < this->width_; j++) {
			const unsigned int pixel (i*this->width_ + j);
			const float pixelMax (this->accumulate_[pixel].getMax());
			if (pixelMax > max) {
				max = pixelMax;
				ii = i;
				jj = j;
			}
		}
	}
	LOG("max = ", max, ", i = ", ii, ", j = ", jj);
	LOG("r = ", this->accumulate_[ii*this->width_ + jj].R_, ", g = ", this->accumulate_[ii*this->width_ + jj].G_, ", b = ", this->accumulate_[ii*this->width_ + jj].B_, " , s = ", this->accumulate_[ii*this->width_ + jj].samples_, " , color = ", this->accumulate_[ii*this->width_ + jj].getColor());

	LOG("point3D = ", Point3D::getInstances());
	LOG("vector3D = ", Vector3D::getInstances());
	LOG("RGB = ", RGB::getInstances());
	LOG("scene = ", Scene::getInstances());
	LOG("ray = ", Ray::getInstances());
	LOG("material = ", Material::getInstances());
	LOG("intersection = ", Intersection::getInstances());
	LOG("FINISH");
}

void Renderer::stopRender() noexcept {
    this->blockSizeX_ = 0u;
    this->blockSizeY_ = 0u;
    this->samplerCamera_->stopSampling();
    this->samplerPixel_->stopSampling();
}

void Renderer::renderScene(unsigned int *const bitmap, const unsigned int tid) noexcept {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelWidth(0.5f / this->width_);
    const float pixelHeight(0.5f / this->height_);
    const auto samples(static_cast<unsigned int> (this->samplerCamera_->samples_));
    RGB pixelRGB;
    Intersection intersection;

    for (unsigned int sample(0u); sample < samples; sample++) {
        do {
			const float block (this->samplerCamera_->getSample(sample));
            if (block >= 1.0f) {break;}
			const auto pixel(static_cast<unsigned int>(static_cast<uint32_t>(::lroundf(block * this->domainSize_)) * this->blockSizeX_ % resolution_));
            const unsigned int startY(((pixel / this->width_) * this->blockSizeY_) % this->height_);
            const unsigned int endY(startY + this->blockSizeY_);
            for (unsigned int y(startY); y < endY; y++) {
                const unsigned int yWidth(y * this->width_);
                const float v(y * INV_IMG_HEIGHT);
                const unsigned int startX((pixel + yWidth) % this->width_);
                const unsigned int endX(startX + this->blockSizeX_);
                for (unsigned int x(startX); x < endX; x++) {
                    const float u(x * INV_IMG_WIDTH);
                    const float r1(this->samplerPixel_->getSample(0u));
                    const float r2(this->samplerPixel_->getSample(0u));
                    const float deviationU((r1 - 0.5f) * 2.0f * pixelWidth);
                    const float deviationV((r2 - 0.5f) * 2.0f * pixelHeight);
                    Ray ray(this->camera_.generateRay(u, v, deviationU, deviationV));
                    pixelRGB.reset(); //pixel color without intersection
                    intersection.length_ = RAY_LENGTH_MAX;
                    intersection.material_ = nullptr;
                    // LOG("triangles = ", shader_.scene_.triangles_.size());
                    // LOG("spheres = ", shader_.scene_.spheres_.size());
                    // LOG("planes = ", shader_.scene_.planes_.size());
                    this->shader_->rayTrace(&pixelRGB, &intersection, std::move(ray));
                    this->accumulate_[yWidth + x].addSampleAndCalcAvg(&pixelRGB);
                    bitmap[yWidth + x] = pixelRGB.RGB2Color();
                }
            }
        } while (true);
        if (tid == 0u) {
			this->sample_ = sample + 1u;
			LOG("Samples terminados = ", this->sample_);
		}
    }
}

unsigned int Renderer::getSample() noexcept {
    return this->sample_;
}
