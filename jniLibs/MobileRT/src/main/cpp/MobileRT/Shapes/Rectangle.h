//
// Created by Tiago on 10-Jul-17.
//

#ifndef MOBILERAYTRACER_RECTANGLE_H
#define MOBILERAYTRACER_RECTANGLE_H

#include "Shape.h"
#include "../Point3D.h"
#include "../Vector3D.h"

namespace MobileRT {
    class Rectangle : public Shape {
    private:
        const Point3D pointA_;
        const Point3D pointB_;
        const Point3D pointC_;
        const Point3D pointD_;
        const Vector3D AB_;
        const Vector3D AC_;
        const Vector3D normal_;

    public:
        /*explicit Rectangle(const Point3D &pointA, const Point3D &pointB,
                          const Point3D &pointC, const Point3D &pointD) noexcept;

        virtual bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept override final;

        virtual void moveTo(const float x, const float y) noexcept override final;

        virtual float getZ(void) const noexcept override final;

        virtual const Point3D getPositionMin(void) const noexcept override final;

        virtual const Point3D getPositionMax(void) const noexcept override final;*/
    };
}

#endif //MOBILERAYTRACER_RECTANGLE_H
