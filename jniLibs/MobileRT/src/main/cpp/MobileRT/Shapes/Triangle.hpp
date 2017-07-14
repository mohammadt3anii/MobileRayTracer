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

        bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept final;

        void moveTo(float x, float y) noexcept final;

        float getZ() const noexcept final;

        const Point3D getPositionMin() const noexcept final;

        const Point3D getPositionMax() const noexcept final;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_TRIANGLE_H
