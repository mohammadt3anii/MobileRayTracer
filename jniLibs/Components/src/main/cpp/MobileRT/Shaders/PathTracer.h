//
// Created by puscas on 20-02-2017.
//

#ifndef MOBILERAYTRACER_PATHTRACER_H
#define MOBILERAYTRACER_PATHTRACER_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Shaders/Shader.h"
#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"
#include <random>
#include <iostream>

namespace MobileRT {
    class PathTracer : public Shader {
    private:
        Sampler &samplerRay_;
        Sampler &samplerLight_;

        void shade(RGB &rgb,
                   Intersection &intersection,
                   Ray &ray) const override;

    public:
        explicit PathTracer(Scene &scene, Sampler &samplerRay, Sampler &samplerPointLight,
                        const unsigned int samplesLight);
    };
}

#endif //MOBILERAYTRACER_PATHTRACER_H
