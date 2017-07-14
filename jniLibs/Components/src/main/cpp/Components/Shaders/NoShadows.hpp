//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_NOSHADOWS_H
#define MOBILERAYTRACER_NOSHADOWS_H

#include "MobileRT/Shader.hpp"

using MobileRT::RGB;
using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::Scene;
namespace Components {
    class NoShadows final : public MobileRT::Shader {
    private:
        virtual void shade(RGB &rgb,
                           const Intersection &intersection,
                           const Ray &) const noexcept override final;

    public:
        explicit NoShadows(Scene &scene, unsigned int samplesLight) noexcept;

        virtual void resetSampling() noexcept override final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_NOSHADOWS_H
