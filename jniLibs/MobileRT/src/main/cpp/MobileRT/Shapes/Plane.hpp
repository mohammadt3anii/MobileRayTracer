//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PLANE_H
#define MOBILERAYTRACER_PLANE_H

#include "Shape.hpp"

namespace MobileRT {
    class Plane final : public Shape {
    private:
        const Point3D point_;   // point in the plane
        const Vector3D normal_;    // normal to the plane

    public:
        explicit Plane(const Point3D &point, const Vector3D &normal) noexcept;

        virtual bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept override final;

        virtual void moveTo(float x, float y) noexcept override final;

        virtual float getZ() const noexcept override final;

        virtual const Point3D getPositionMin() const noexcept override final;

        virtual const Point3D getPositionMax() const noexcept override final;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_PLANE_H
