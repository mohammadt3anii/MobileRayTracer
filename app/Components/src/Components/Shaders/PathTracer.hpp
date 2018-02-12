//
// Created by puscas on 20-02-2017.
//

#ifndef COMPONENTS_SHADERS_PATHTRACER_HPP
#define COMPONENTS_SHADERS_PATHTRACER_HPP

#include "MobileRT/Sampler.hpp"
#include "MobileRT/Shader.hpp"
#include <iostream>
#include <memory>
#include <random>

namespace Components {
    using ::MobileRT::Sampler;
    using ::MobileRT::RGB;
    using ::MobileRT::Intersection;
    using ::MobileRT::Ray;
    using ::MobileRT::Scene;
    using ::MobileRT::Vector3D;

    class PathTracer final : public ::MobileRT::Shader {
    private:
        ::std::unique_ptr<Sampler> samplerRay_{};
        ::std::unique_ptr<Sampler> samplerLight_{};
        ::std::unique_ptr<Sampler> samplerRussianRoulette_{};

    private:
        bool shade(RGB *rgb,
                   Intersection intersection,
                   Ray &&ray) noexcept final;

        Vector3D getCosineSampleHemisphere(Vector3D normal) const noexcept;

    public:
        explicit PathTracer(Scene &&scene,
                            ::std::unique_ptr<Sampler> &&samplerRay,
                            ::std::unique_ptr<Sampler> &&samplerLight,
                            ::std::unique_ptr<Sampler> &&samplerRussianRoulette,
                            unsigned samplesLight, Accelerator accelerator) noexcept;

        PathTracer(const PathTracer &pathTracer) noexcept = delete;

        PathTracer(PathTracer &&pathTracer) noexcept = delete;

        ~PathTracer() noexcept final = default;

        PathTracer &operator=(const PathTracer &pathTracer) noexcept = delete;

        PathTracer &operator=(PathTracer &&pathTracer) noexcept = delete;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SHADERS_PATHTRACER_HPP