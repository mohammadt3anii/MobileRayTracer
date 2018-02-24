//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_LIGHTS_POINTLIGHT_HPP
#define COMPONENTS_LIGHTS_POINTLIGHT_HPP

#include "MobileRT/Light.hpp"

namespace Components {
    using ::MobileRT::Material;
    using ::MobileRT::Intersection;
    using ::MobileRT::Ray;

    class PointLight final : public ::MobileRT::Light {
    private:
        ::glm::vec3 position_{};

    public:
        explicit PointLight(Material radiance, ::glm::vec3 position) noexcept;

        PointLight(const PointLight &pointLight) noexcept = delete;

        PointLight(PointLight &&pointLight) noexcept = delete;

        ~PointLight() noexcept final = default;

        PointLight &operator=(const PointLight &pointLight) noexcept = delete;

        PointLight &operator=(PointLight &&pointLight) noexcept = delete;

        ::glm::vec3 getPosition() noexcept final;

        void resetSampling() noexcept final;

        Intersection intersect(Intersection intersection, Ray ray) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_LIGHTS_POINTLIGHT_HPP
