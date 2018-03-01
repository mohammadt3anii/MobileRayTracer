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

    class PathTracer final : public ::MobileRT::Shader {
    private:
        ::std::unique_ptr<::MobileRT::Sampler> samplerRay_{};
        ::std::unique_ptr<::MobileRT::Sampler> samplerLight_{};
        ::std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette_{};

    private:
        bool shade(
            ::glm::vec3 *rgb,
            ::MobileRT::Intersection intersection,
            ::MobileRT::Ray ray) noexcept final;

        ::glm::vec3 getCosineSampleHemisphere(::glm::vec3 normal) const noexcept;

    public:
        explicit PathTracer(::MobileRT::Scene scene,
                            ::std::unique_ptr<::MobileRT::Sampler> samplerRay,
                            ::std::unique_ptr<::MobileRT::Sampler> samplerLight,
                            ::std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette,
                            ::std::uint32_t samplesLight, Accelerator accelerator) noexcept;

        PathTracer(const PathTracer &pathTracer) noexcept = delete;

        PathTracer(PathTracer &&pathTracer) noexcept = delete;

        ~PathTracer() noexcept final = default;

        PathTracer &operator=(const PathTracer &pathTracer) noexcept = delete;

        PathTracer &operator=(PathTracer &&pathTracer) noexcept = delete;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SHADERS_PATHTRACER_HPP
