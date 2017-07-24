//
// Created by puscas on 03-03-2017.
//

#ifndef COMPONENTS_LIGHTS_AREALIGHT_HPP
#define COMPONENTS_LIGHTS_AREALIGHT_HPP

#include "MobileRT/Light.hpp"
#include "MobileRT/Sampler.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include "MobileRT/Utils.hpp"

namespace Components {
	using MobileRT::Material;
	using MobileRT::Sampler;
	using MobileRT::Point3D;
	using MobileRT::Ray;
	using MobileRT::Intersection;
	using MobileRT::Triangle;
    class AreaLight final : public MobileRT::Light
		//, public MobileRT::Triangle
		{
    private:
				Triangle triangle_;
        Sampler &samplerPointLight_;

    public:
        explicit AreaLight(const Material &radiance,
                           Sampler &samplerPointLight,
                           const Point3D &pointA,
                           const Point3D &pointB,
                           const Point3D &pointC) noexcept;

		AreaLight(const AreaLight &areaLight) noexcept = delete;

		AreaLight(AreaLight &&areaLight) noexcept = delete;

		~AreaLight() noexcept final = default;

        AreaLight &operator=(const AreaLight &areaLight) noexcept = delete;

		AreaLight &operator=(AreaLight &&areaLight) noexcept = delete;

        Point3D getPosition() noexcept final;

        void resetSampling() noexcept final;

		//using MobileRT::Triangle::intersect;
        bool intersect(Intersection &intersection, const Ray &ray) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_LIGHTS_AREALIGHT_HPP
