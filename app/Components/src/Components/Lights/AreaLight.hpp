//
// Created by puscas on 03-03-2017.
//

#ifndef COMPONENTS_LIGHTS_AREALIGHT_HPP
#define COMPONENTS_LIGHTS_AREALIGHT_HPP

#include "MobileRT/Light.hpp"
#include "MobileRT/Sampler.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include "MobileRT/Utils.hpp"
#include <memory>

namespace Components {
    using ::MobileRT::Material;
    using ::MobileRT::Sampler;
    using ::MobileRT::Point3D;
    using ::MobileRT::Ray;
    using ::MobileRT::Intersection;
    using ::MobileRT::Triangle;

    class AreaLight final : public ::MobileRT::Light {
    private:
        Triangle triangle_;
        ::std::unique_ptr<Sampler> samplerPointLight_{};

    public:
        explicit AreaLight(Material radiance,
                           ::std::unique_ptr<Sampler> &&samplerPointLight,
                           Point3D pointA,
                           Point3D pointB,
                           Point3D pointC) noexcept;

        AreaLight(const AreaLight &areaLight) noexcept = delete;

        AreaLight(AreaLight &&areaLight) noexcept = delete;

        ~AreaLight() noexcept final = default;

        AreaLight &operator=(const AreaLight &areaLight) noexcept = delete;

        AreaLight &operator=(AreaLight &&areaLight) noexcept = delete;

        Point3D getPosition() noexcept final;

        void resetSampling() noexcept final;

        bool intersect(Intersection *intersection, Ray ray) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_LIGHTS_AREALIGHT_HPP
