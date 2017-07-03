//
// Created by Tiago on 15-11-2016.
//

#ifndef MOBILERAYTRACER_TRIANGLE_H
#define MOBILERAYTRACER_TRIANGLE_H

#include "Shape.h"
#include "../Point3D.h"
#include "../Vector3D.h"

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
                               const Material &material) const noexcept override;

        virtual void moveTo(const float x, const float y) noexcept override final;

        virtual float getZ(void) const noexcept override final;
    };
}

#endif //MOBILERAYTRACER_TRIANGLE_H
