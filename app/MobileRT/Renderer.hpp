//
// Created by Tiago on 09-Feb-17.
//

#ifndef MOBILERT_RENDERER_HPP
#define MOBILERT_RENDERER_HPP

#include "Camera.hpp"
#include "Sampler.hpp"
#include "Shader.hpp"
#include "Utils.hpp"
#include <cmath>
#include <thread>

namespace MobileRT {
    class Renderer final {
    public:
        ::std::unique_ptr<Camera> camera_{};
        ::std::unique_ptr<Shader> shader_{};

    private:
        ::std::unique_ptr<Sampler> samplerPixel_{};
        uint32_t blockSizeX_{};
        uint32_t blockSizeY_{};
        uint32_t sample_{};
        const uint32_t width_{};
        const uint32_t height_{};
        const uint32_t domainSize_{};
        const uint32_t resolution_{};
        const uint32_t samplesPixel_{};

    private:
        void renderScene(uint32_t *bitmap, int32_t tid, uint32_t width) noexcept;

    public:
        explicit Renderer(::std::unique_ptr<Shader> shader,
                          ::std::unique_ptr<Camera> camera,
                          ::std::unique_ptr<Sampler> samplerPixel,
                          uint32_t width, uint32_t height,
                          uint32_t samplesPixel) noexcept;

        Renderer(const Renderer &renderer) noexcept = delete;

        Renderer(Renderer &&renderer) noexcept = delete;

        ~Renderer() noexcept = default;

        Renderer &operator=(const Renderer &renderer) noexcept = delete;

        Renderer &operator=(Renderer &&renderer) noexcept = delete;

        void renderFrame(uint32_t *bitmap, int32_t numThreads, uint32_t stride) noexcept;

        void stopRender() noexcept;

        uint32_t getSample() const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_RENDERER_HPP
