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
		public:
    Camera *const camera_ {};
    Shader *const shader_ {};

    private:
    Sampler *const samplerPixel_ {};
      std::vector<RGB> accumulate_ {};
    unsigned blockSizeX_ {};
    unsigned blockSizeY_ {};
    unsigned sample_ {};
    const unsigned width_ {};
    const unsigned height_ {};
    const unsigned domainSize_ {};
    const unsigned resolution_ {};
    const unsigned samplesPixel_ {};

    private:
    void renderScene (unsigned *bitmap, int tid) noexcept;

    public:
    explicit Renderer (std::unique_ptr<Shader> &&shader,
                       std::unique_ptr<Camera> &&camera,
                       std::unique_ptr<Sampler> &&samplerPixel,
                       unsigned width, unsigned height,
                       unsigned samplesPixel) noexcept;
    Renderer (const Renderer &renderer) noexcept = delete;
    Renderer (Renderer &&renderer) noexcept = delete;
    ~Renderer () noexcept = default;
    Renderer &operator= (const Renderer &renderer) noexcept = delete;
    Renderer &operator= (Renderer &&renderer) noexcept = delete;
    void renderFrame (unsigned *bitmap, int numThreads) noexcept;
    void stopRender () noexcept;
    unsigned getSample () const noexcept;
  };
}//namespace MobileRT

#endif //MOBILERT_RENDERER_HPP
