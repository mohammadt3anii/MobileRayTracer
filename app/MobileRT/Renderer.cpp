//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.hpp"

using ::MobileRT::Renderer;

Renderer::Renderer(::std::unique_ptr<Shader> shader,
                   ::std::unique_ptr<Camera> camera,
                   ::std::unique_ptr<Sampler> samplerPixel,
                   const unsigned width, const unsigned height,
                   const unsigned samplesPixel) noexcept :
        camera_{::std::move(camera)},
        shader_{::std::move(shader)},
        samplerPixel_{::std::move(samplerPixel)},
        blockSizeX_{width / static_cast<unsigned>(::std::sqrt(NumberOfBlocks))},
        blockSizeY_{height / static_cast<unsigned>(::std::sqrt(NumberOfBlocks))},
        sample_{0},
        width_{width},
        height_{height},
        domainSize_{(width / blockSizeX_) * (height / blockSizeY_)},
        resolution_{width * height},
        samplesPixel_{samplesPixel} {
    this->shader_->initializeAccelerators(camera_.get());
}

void Renderer::renderFrame(unsigned *const bitmap, const int numThreads,
                           const unsigned stride) noexcept {
    LOG("numThreads = ", numThreads);
    const unsigned realWidth {stride / static_cast<unsigned>(sizeof(unsigned))};
    LOG("realWidth = ", realWidth);
    LOG("width_ = ", width_);

    this->sample_ = 0;
    this->samplerPixel_->resetSampling();
    this->shader_->resetSampling();
    this->camera_->resetSampling();

    const int numChildren{numThreads - 1};
    ::std::vector<::std::thread> threads {};
    threads.reserve(static_cast<unsigned>(numChildren));

    for (int i{0}; i < numChildren; i++) {
        threads.emplace_back(&Renderer::renderScene, this, bitmap, i, realWidth);
    }
    renderScene(bitmap, numChildren, realWidth);
    for (::std::thread &thread : threads) {
        thread.join();
    }
    threads.clear();

    /*LOG("scene = ", Scene::getInstances());
    LOG("ray = ", Ray::getInstances());
    LOG("material = ", Material::getInstances());
    LOG("intersection = ", Intersection::getInstances());*/
    LOG("Resolution = ", width_, "x", height_);
    LOG("FINISH");
}

void Renderer::stopRender() noexcept {
    this->blockSizeX_ = 0;
    this->blockSizeY_ = 0;
    this->samplerPixel_->stopSampling();
}

void Renderer::renderScene(unsigned *const bitmap, const int tid, const unsigned width) noexcept {
    const float INV_IMG_WIDTH{1.0f / this->width_};
    const float INV_IMG_HEIGHT{1.0f / this->height_};
    const float pixelWidth{0.5f / this->width_};
    const float pixelHeight{0.5f / this->height_};
    const unsigned samples{this->samplesPixel_};
    glm::vec3 pixelRGB {};
    for (unsigned sample{0}; sample < samples; sample++) {
        while (true) {
            const float block{this->camera_->getBlock(sample)};
            if (block >= 1.0f) { break; }
            const unsigned pixel{
                    static_cast<unsigned>(static_cast<uint32_t>(roundf(block * this->domainSize_)) *
                                          this->blockSizeX_ % resolution_)};
            const unsigned startY{
                    ((pixel / this->width_) * this->blockSizeY_) % this->height_};
            const unsigned endY{startY + this->blockSizeY_};
            for (unsigned y{startY}; y < endY; y++) {
                //const unsigned yWidth{y * this->width_};
                const unsigned yWidth{y * width};
                const float v{y * INV_IMG_HEIGHT};
                const unsigned startX{(pixel + yWidth) % this->width_};
                const unsigned endX{startX + this->blockSizeX_};
                for (unsigned x{startX}; x < endX; x++) {
                    const float u{x * INV_IMG_WIDTH};
                    const float r1{this->samplerPixel_->getSample()};
                    const float r2{this->samplerPixel_->getSample()};
                    const float deviationU{(r1 - 0.5f) * 2.0f * pixelWidth};
                    const float deviationV{(r2 - 0.5f) * 2.0f * pixelHeight};
                    Ray ray{this->camera_->generateRay(u, v, deviationU, deviationV)};
                    pixelRGB = {};
                    this->shader_->rayTrace(&pixelRGB, ::std::move(ray));
                    const unsigned pixelIndex{yWidth + x};
                    if (pixelIndex >= width * height_) {
                        LOG("PASSOU O LIMITE DA RESOLUÇÃO");
                    }
                    bitmap[pixelIndex] = ::MobileRT::incrementalAvg(pixelRGB, bitmap[pixelIndex],
                        sample + 1);
                }
            }
        }
        if (tid == 0) {
            this->sample_ = sample + 1;
            LOG("Samples terminados = ", this->sample_);
        }
    }
}

unsigned Renderer::getSample() const noexcept {
    return this->sample_;
}
