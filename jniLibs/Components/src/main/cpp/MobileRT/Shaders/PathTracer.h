//
// Created by puscas on 20-02-2017.
//

#ifndef MOBILERAYTRACER_PATHTRACER_H
#define MOBILERAYTRACER_PATHTRACER_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Shaders/Shader.h"
#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"


namespace MobileRT {
    class PathTracer : public Shader {
    private:
        Sampler &sampler_;

        void shade(RGB &rgb,
                   Intersection &intersection,
                   const Ray &ray) const override;

    public:
        explicit PathTracer(const Scene &scene, Sampler &sampler);
    };
};


#endif //MOBILERAYTRACER_PATHTRACER_H
