//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "Shape.h"

namespace MobileRT {
    class Sphere final : public Shape {
    private:
        const float sq_radius_;
        Point3D center_;

    public:
        explicit Sphere(const Point3D &center, const float radius) noexcept;

        virtual bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept override final;

        virtual void moveTo(const float x, const float y) noexcept override final;

        virtual float getZ(void) const noexcept override final;

        virtual const Point3D getPositionMin(void) const noexcept override final;

        virtual const Point3D getPositionMax(void) const noexcept override final;
    };
}

#endif //MOBILERAYTRACER_SPHERE_H
