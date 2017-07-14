//
// Created by Tiago on 10-Jul-17.
//

#ifndef MOBILERAYTRACER_RECTANGLE_H
#define MOBILERAYTRACER_RECTANGLE_H

#include "Shape.hpp"
#include "../Point3D.hpp"
#include "../Vector3D.hpp"

namespace MobileRT {
    class Rectangle : public Shape {
    private:
        Point3D pointA_;
        Point3D pointB_;
        Point3D pointC_;
        Point3D pointD_;
        Vector3D AB_;
        Vector3D AC_;
        Vector3D normal_;

    public:
        explicit Rectangle(const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) noexcept;

        bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept final;

        void moveTo(float x, float y) noexcept final;

        float getZ() const noexcept final;

        const Point3D getPositionMin() const noexcept final;

        const Point3D getPositionMax() const noexcept final;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_RECTANGLE_H