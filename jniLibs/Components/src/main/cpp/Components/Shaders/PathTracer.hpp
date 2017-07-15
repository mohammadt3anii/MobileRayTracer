//
// Created by puscas on 20-02-2017.
//

#ifndef MOBILERAYTRACER_PATHTRACER_H
#define MOBILERAYTRACER_PATHTRACER_H

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
                           const Intersection &intersection,
                           const Ray &ray) const noexcept final;

    public:
        explicit PathTracer(Scene &scene,
                            Sampler &samplerRay, Sampler &samplerLight,
                            unsigned int samplesLight) noexcept;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_PATHTRACER_H
