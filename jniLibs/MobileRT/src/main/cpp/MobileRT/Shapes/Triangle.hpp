//
// Created by Tiago on 15-11-2016.
//

#ifndef MOBILERAYTRACER_TRIANGLE_H
#define MOBILERAYTRACER_TRIANGLE_H

#include "Shape.hpp"
#include "../Point3D.hpp"
#include "../Vector3D.hpp"

namespace MobileRT {
    class Triangle : public Shape {
    public:
        const Point3D pointA_;
        const Point3D pointB_;
        const Point3D pointC_;
        const Vector3D AB_;
        const Vector3D AC_;
        const Vector3D normal_;

    public:
        explicit Triangle(const Point3D &pointA, const Point3D &pointB,
                          const Point3D &pointC) noexcept;

        virtual bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept override final;

        virtual void moveTo(float x, float y) noexcept override final;

        virtual float getZ() const noexcept override final;

        virtual const Point3D getPositionMin() const noexcept override final;

        virtual const Point3D getPositionMax() const noexcept override final;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_TRIANGLE_H
