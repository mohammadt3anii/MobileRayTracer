//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINTLIGHT_H
#define MOBILERAYTRACER_POINTLIGHT_H

#include "MobileRT/Light.hpp"

using MobileRT::Material;
using MobileRT::Point3D;
using MobileRT::Intersection;
using MobileRT::Ray;
namespace Components {
    class PointLight final : public MobileRT::Light {
    private:
        Point3D position_;

    public:
        explicit PointLight(const Material &radiance, const Point3D &position) noexcept;

        virtual const Point3D getPosition() noexcept override final;

        virtual void resetSampling() noexcept override final;

        virtual bool
        intersectL(Intersection &intersection, const Ray &ray) const noexcept override final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_POINTLIGHT_H
