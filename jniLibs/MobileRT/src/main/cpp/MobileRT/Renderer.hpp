//
// Created by Tiago on 09-Feb-17.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include "Camera.hpp"
#include "RGB.hpp"
#include "Sampler.hpp"
#include "Shader.hpp"
#include "Utils.hpp"
#include <cmath>
#include <thread>

namespace MobileRT {
    class Renderer final {
    private:
        Sampler &samplerCamera_;
        Shader &shader_;
        const Camera &camera_;
        const unsigned int width_;
        const unsigned int height_;
		std::vector<RGB> accumulate_;
        const unsigned int domainSize_;
        unsigned int blockSizeX_;
        unsigned int blockSizeY_;
        const unsigned int resolution_;
        Sampler &samplerPixel_;
        unsigned int sample_;
        std::function<float(float)> toneMapper_;

    private:
        void renderScene(unsigned int *bitmap, unsigned int tid) noexcept;

        void toneMapper(RGB &pixel) noexcept;

    public:
        explicit Renderer(Sampler &samplerCamera, Shader &shader,
                          Camera &camera,
                          unsigned int width, unsigned int height,
                          unsigned int blockSizeX,
                          unsigned int blockSizeY,
                          Sampler &samplerPixel) noexcept;

		Renderer(const Renderer &renderer) noexcept = delete;

		Renderer(Renderer &&renderer) noexcept = delete;

        ~Renderer() noexcept = default;

		Renderer &operator=(const Renderer &renderer) const noexcept = delete;

		Renderer &&operator=(Renderer &&renderer) const noexcept = delete;

        void renderFrame(unsigned int *bitmap, unsigned int numThreads) noexcept;

        void registerToneMapper(std::function<float(float value)> toneMapper) noexcept;

        void stopRender() noexcept;

        unsigned int getSample() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_RENDERER_H
