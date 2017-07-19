//
// Created by puscas on 20-02-2017.
//

#ifndef COMPONENTS_SHADERS_PATHTRACER_HPP
#define COMPONENTS_SHADERS_PATHTRACER_HPP

#include "MobileRT/Sampler.hpp"
#include "MobileRT/Shader.hpp"
#include <iostream>
#include <random>

namespace Components {
	using MobileRT::Sampler;
	using MobileRT::RGB;
	using MobileRT::Intersection;
	using MobileRT::Ray;
	using MobileRT::Scene;
    class PathTracer final : public MobileRT::Shader {
    private:
        Sampler &samplerRay_;
        Sampler &samplerLight_;

    private:
			void shade(RGB &rgb,
								 Intersection &&intersection,
								 Ray &&ray) const noexcept final;

		public:
        explicit PathTracer(Scene &scene,
                            Sampler &samplerRay, Sampler &samplerLight,
                            unsigned int samplesLight) noexcept;

		PathTracer(const PathTracer &pathTracer) noexcept = delete;

		PathTracer(PathTracer &&pathTracer) noexcept = delete;

		~PathTracer() noexcept final = default;

        PathTracer &operator=(const PathTracer &pathTracer) noexcept = delete;

		PathTracer &operator=(PathTracer &&pathTracer) noexcept = delete;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SHADERS_PATHTRACER_HPP
