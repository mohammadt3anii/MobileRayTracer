//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.hpp"

using MobileRT::Renderer;

Renderer::Renderer(Sampler &samplerCamera, Shader &shader, Camera &camera,
                   const unsigned int width, const unsigned int height,
                   const unsigned int blockSizeX, const unsigned int blockSizeY,
                   Sampler &samplerPixel) noexcept :
        samplerCamera_(samplerCamera),
        shader_(shader),
        camera_(camera),
        width_(width),
        height_(height),
		accumulate_(std::vector<RGB> (width * height)),
        domainSize_((width / blockSizeX) * (height / blockSizeY)),
        blockSizeX_(blockSizeX),
        blockSizeY_(blockSizeY),
        resolution_(width * height),
        samplerPixel_(samplerPixel),
        sample_(0u),
        toneMapper_([&](const float value) { return value; }) {
}

void Renderer::renderFrame(unsigned int *const bitmap, const unsigned int numThreads) noexcept {
    LOG("START - resolution = ", resolution_);
    this->sample_ = 0u;
    const unsigned int size(this->width_ * this->height_);
    for (unsigned int i(0u); i < size; i++) {
        this->accumulate_[i].reset();
    }
    this->samplerCamera_.resetSampling();
    this->samplerPixel_.resetSampling();
    this->shader_.resetSampling();
    const unsigned numChildren (numThreads - 1u);
	std::vector<std::thread> threads;
	threads.reserve(numChildren);
    for (unsigned int i(0u); i < numChildren; i++) {
		threads.emplace_back(std::thread(&Renderer::renderScene, this, bitmap, i));
    }
    renderScene(bitmap, numChildren);
    for (unsigned int i(0u); i < numChildren; i++) {
        threads[i].join();
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
	const unsigned int pixel2 (ii*this->width_ + jj);
	RGB& pixelRGB2 (this->accumulate_[pixel2]);
	LOG("max = ", max, ", i = ", ii, ", j = ", jj);
	LOG("r = ", pixelRGB2.R_, ", g = ", pixelRGB2.G_, ", b = ", pixelRGB2.B_, " , s = ", pixelRGB2.samples_, " , color = ", pixelRGB2.RGB2Color2());

    LOG("point3D = ", Point3D::getInstances());
    LOG("vector3D = ", Vector3D::getInstances());
    LOG("RGB = ", RGB::getInstances());
    LOG("scene = ", Scene::getInstances());
    LOG("ray = ", Ray::getInstances());
    LOG("material = ", Material::getInstances());
    LOG("intersection = ", Intersection::getInstances());
    LOG("FINISH", "");
}

void Renderer::registerToneMapper(std::function<float(const float)> toneMapper) noexcept {
    toneMapper_ = std::move(toneMapper);
}

void Renderer::stopRender() noexcept {
    this->blockSizeX_ = 0u;
    this->blockSizeY_ = 0u;
    this->samplerCamera_.stopSampling();
    this->samplerPixel_.stopSampling();
}

void Renderer::toneMapper(RGB &pixel) noexcept {
    pixel.R_ = toneMapper_(pixel.R_);
    pixel.G_ = toneMapper_(pixel.G_);
    pixel.B_ = toneMapper_(pixel.B_);
}

void Renderer::renderScene(unsigned int *const bitmap, const unsigned int tid) noexcept {
    const float INV_IMG_WIDTH(1.0f / this->width_);
    const float INV_IMG_HEIGHT(1.0f / this->height_);
    const float pixelWidth(0.5f / this->width_);
    const float pixelHeight(0.5f / this->height_);
    const auto samples(static_cast<unsigned int> (this->samplerCamera_.samples_));
    Intersection intersection;
    RGB pixelRGB;

    for (unsigned int sample(0u); sample < samples; sample++) {
        for (;;) {
			const float block (this->samplerCamera_.getSample(sample));
            if (block >= 1.0f) {break;}
			const auto pixel(static_cast<unsigned int>(static_cast<uint32_t>(::lroundf(block * this->domainSize_)) * this->blockSizeX_ % resolution_));
            const unsigned int startY(((pixel / width_) * blockSizeY_) % height_);
            const unsigned int endY(startY + this->blockSizeY_);
            for (unsigned int y(startY); y < endY; y++) {
                const unsigned int yWidth(y * width_);
                const float v(y * INV_IMG_HEIGHT);
                const unsigned int startX((pixel + yWidth) % width_);
                const unsigned int endX(startX + this->blockSizeX_);
                for (unsigned int x(startX); x < endX; x++) {
                    const float u(x * INV_IMG_WIDTH);
                    const float r1(this->samplerPixel_.getSample(0u));
                    const float r2(this->samplerPixel_.getSample(0u));
                    const float deviationU((r1 - 0.5f) * 2.0f * pixelWidth);
                    const float deviationV((r2 - 0.5f) * 2.0f * pixelHeight);
                    const Ray &ray(this->camera_.generateRay(u, v, deviationU, deviationV));
                    intersection.length_ = RAY_LENGTH_MAX;
                    this->shader_.rayTrace(pixelRGB, ray, intersection);
                    this->accumulate_[yWidth + x].addSampleAndCalcAvg(pixelRGB);
                    toneMapper(pixelRGB);
                    bitmap[yWidth + x] = pixelRGB.RGB2Color();
                }
            }
        }
        if (tid == 0u) {
			this->sample_ = sample + 1u;
			LOG("Samples terminados = ", this->sample_);
		}
    }
}

unsigned int Renderer::getSample() noexcept {
    return this->sample_;
}
