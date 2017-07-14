//
// Created by puscas on 03-03-2017.
//

#ifndef MOBILERAYTRACER_AREALIGHT_H
#define MOBILERAYTRACER_AREALIGHT_H

#include "MobileRT/Light.hpp"
#include "MobileRT/Sampler.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include "MobileRT/Utils.hpp"

using MobileRT::Material;
using MobileRT::Sampler;
using MobileRT::Point3D;
using MobileRT::Ray;
using MobileRT::Intersection;
namespace Components {
    class AreaLight final : public MobileRT::Light, public MobileRT::Triangle {
    private:
        Sampler &samplerPointLight_;

    public:
        explicit AreaLight(const Material &radiance,
                           Sampler &samplerPointLight,
                           const Point3D &pointA,
                           const Point3D &pointB,
                           const Point3D &pointC) noexcept;

        const Point3D getPosition() noexcept final;

        void resetSampling() noexcept final;

        bool
        intersectL(Intersection &intersection, const Ray &ray) const noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_AREALIGHT_H
