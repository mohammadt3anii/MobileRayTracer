//
// Created by puscas on 31/07/17.
//

#ifndef MOBILERAYTRACER_DIFFUSEMATERIAL_HPP
#define MOBILERAYTRACER_DIFFUSEMATERIAL_HPP

#include "MobileRT/Shader.hpp"

namespace Components {
    using MobileRT::RGB;
    using MobileRT::Intersection;
    using MobileRT::Ray;
    using MobileRT::Scene;

    class DiffuseMaterial : public MobileRT::Shader {

    private:
        bool shade(RGB *rgb,
                   Intersection const &intersection,
                   Ray && /*ray*/) const noexcept final;

    public:
        explicit DiffuseMaterial(Scene &&scene) noexcept;

        DiffuseMaterial(const DiffuseMaterial &diffuseMaterial) noexcept = delete;

        DiffuseMaterial(DiffuseMaterial &&diffuseMaterial) noexcept = delete;

        ~DiffuseMaterial() noexcept final = default;

        DiffuseMaterial &operator=(const DiffuseMaterial &diffuseMaterial) noexcept = delete;

        DiffuseMaterial &operator=(DiffuseMaterial &&diffuseMaterial) noexcept = delete;

        void resetSampling() noexcept final;
    };
}

#endif //MOBILERAYTRACER_DIFFUSEMATERIAL_HPP
