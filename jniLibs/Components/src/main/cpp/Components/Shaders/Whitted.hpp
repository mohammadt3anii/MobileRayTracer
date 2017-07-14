//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_WHITTED_H
#define MOBILERAYTRACER_WHITTED_H

#include "MobileRT/Shader.hpp"

using MobileRT::RGB;
using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::Scene;
namespace Components {
    class Whitted final : public MobileRT::Shader {
    private:
        virtual void shade(RGB &rgb,
                           const Intersection &intersection,
                           const Ray &ray) const noexcept override final;

    public:
        explicit Whitted(Scene &scene, unsigned int samplesLight) noexcept;

        virtual void resetSampling() noexcept override final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_WHITTED_H
