//
// Created by puscas on 31/07/17.
//

#ifndef MOBILERAYTRACER_DIFFUSEMATERIAL_HPP
#define MOBILERAYTRACER_DIFFUSEMATERIAL_HPP

#include "MobileRT/Shader.hpp"

namespace Components {
    using ::MobileRT::Intersection;
    using ::MobileRT::Ray;
    using ::MobileRT::Scene;

    class DiffuseMaterial final : public ::MobileRT::Shader {
    private:
        bool shade(::glm::vec3 *rgb, Intersection intersection, Ray ray) noexcept final;

    public:
        explicit DiffuseMaterial(Scene scene, Accelerator accelerator) noexcept;

        DiffuseMaterial(const DiffuseMaterial &diffuseMaterial) noexcept = delete;

        DiffuseMaterial(DiffuseMaterial &&diffuseMaterial) noexcept = delete;

        ~DiffuseMaterial() noexcept final = default;

        DiffuseMaterial &operator=(const DiffuseMaterial &diffuseMaterial) noexcept = delete;

        DiffuseMaterial &operator=(DiffuseMaterial &&diffuseMaterial) noexcept = delete;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_DIFFUSEMATERIAL_HPP
