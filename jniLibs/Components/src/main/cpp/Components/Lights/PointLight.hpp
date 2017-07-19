//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_LIGHTS_POINTLIGHT_HPP
#define COMPONENTS_LIGHTS_POINTLIGHT_HPP

#include "MobileRT/Light.hpp"

namespace Components {
	using MobileRT::Material;
	using MobileRT::Point3D;
	using MobileRT::Intersection;
	using MobileRT::Ray;
    class PointLight final : public MobileRT::Light {
    private:
        Point3D position_;

    public:
        explicit PointLight(const Material &radiance, const Point3D &position) noexcept;

		PointLight(const PointLight &pointLight) noexcept = delete;

		PointLight(PointLight &&pointLight) noexcept = delete;

		~PointLight() noexcept final = default;

        PointLight &operator=(const PointLight &pointLight) noexcept = delete;

		PointLight &operator=(PointLight &&pointLight) noexcept = delete;

        Point3D getPosition() noexcept final;

        void resetSampling() noexcept final;

        bool
        intersect(Intersection &intersection, const Ray &ray) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_LIGHTS_POINTLIGHT_HPP
