//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINTLIGHT_H
#define MOBILERAYTRACER_POINTLIGHT_H

#include "MobileRT/Light.h"

namespace Components {
    using namespace MobileRT;

    class PointLight final : public Light {
    private:
        Point3D position_;

    public:
        explicit PointLight(const Material &radiance, const Point3D &position) noexcept;

        virtual const Point3D getPosition(void) noexcept override final;

        virtual void resetSampling(void) noexcept override final;

        virtual bool
        intersectL(Intersection &intersection, const Ray &ray) const noexcept override final;
    };
}

#endif //MOBILERAYTRACER_POINTLIGHT_H
