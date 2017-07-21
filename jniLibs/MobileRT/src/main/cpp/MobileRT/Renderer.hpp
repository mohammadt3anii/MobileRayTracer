//
// Created by Tiago on 09-Feb-17.
//

#ifndef MOBILERT_RENDERER_HPP
#define MOBILERT_RENDERER_HPP

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
		Sampler &samplerPixel_;
		Camera &camera_;
        Shader &shader_;
		std::vector<RGB> accumulate_;
		const unsigned int domainSize_;
        const unsigned int width_;
        const unsigned int height_;
        unsigned int blockSizeX_;
        unsigned int blockSizeY_;
		const unsigned int resolution_;
        unsigned int sample_;

    private:
        void renderScene(unsigned int *bitmap, unsigned int tid) noexcept;

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

		Renderer &operator=(const Renderer &renderer) noexcept = delete;

		Renderer &operator=(Renderer &&renderer) noexcept = delete;

        void renderFrame(unsigned int *bitmap, unsigned int numThreads) noexcept;

        void stopRender() noexcept;

        unsigned int getSample() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_RENDERER_HPP
