//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_NOSHADOWS_H
#define MOBILERAYTRACER_NOSHADOWS_H

#include "MobileRT/Shader.hpp"

namespace Components {
	using MobileRT::RGB;
	using MobileRT::Intersection;
	using MobileRT::Ray;
	using MobileRT::Scene;
    class NoShadows final : public MobileRT::Shader {
    private:
        void shade(RGB &rgb,
                           const Intersection &intersection,
                           const Ray &/*ray*/) const noexcept final;

    public:
        explicit NoShadows(Scene &scene, unsigned int samplesLight) noexcept;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_NOSHADOWS_H
