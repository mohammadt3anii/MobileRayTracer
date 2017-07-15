//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_SHADERS_NOSHADOWS_HPP
#define COMPONENTS_SHADERS_NOSHADOWS_HPP

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

#endif //COMPONENTS_SHADERS_NOSHADOWS_HPP
