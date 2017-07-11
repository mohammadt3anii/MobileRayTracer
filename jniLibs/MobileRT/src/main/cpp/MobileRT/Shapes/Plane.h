//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PLANE_H
#define MOBILERAYTRACER_PLANE_H

#include "Shape.h"

namespace MobileRT {
    class Plane final : public Shape {
    private:
        const Point3D point_;   // point in the plane
        const Vector3D normal_;    // normal to the plane

    public:
        explicit Plane(const Point3D &point, const Vector3D &normal) noexcept;

        virtual bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept override final;

        virtual void moveTo(const float x, const float y) noexcept override final;

        virtual float getZ(void) const noexcept override final;

        virtual const Point3D getPositionMin(void) const noexcept override final;

        virtual const Point3D getPositionMax(void) const noexcept override final;
    };
}

#endif //MOBILERAYTRACER_PLANE_H
