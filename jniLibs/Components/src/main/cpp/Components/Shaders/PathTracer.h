//
// Created by puscas on 20-02-2017.
//

#ifndef MOBILERAYTRACER_PATHTRACER_H
#define MOBILERAYTRACER_PATHTRACER_H

#include "MobileRT/Shader.h"
#include "MobileRT/Sampler.h"
#include <random>
#include <iostream>

namespace Components {
    using namespace MobileRT;

    class PathTracer final : public Shader {
    private:
        Sampler &samplerRay_;
        Sampler &samplerLight_;

    private:
        virtual void shade(RGB &rgb,
                           const Intersection &intersection,
                           const Ray &ray) const noexcept override final;

    public:
        explicit PathTracer(Scene &scene,
                            Sampler &samplerRay, Sampler &samplerPointLight,
                            const unsigned int samplesLight) noexcept;

        virtual void resetSampling(void) noexcept override final;
    };
}

#endif //MOBILERAYTRACER_PATHTRACER_H
