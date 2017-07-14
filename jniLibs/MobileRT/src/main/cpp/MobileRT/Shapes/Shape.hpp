//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHAPE_H
#define MOBILERAYTRACER_SHAPE_H

#include "../Intersection.hpp"
#include "../Material.hpp"
#include "../Ray.hpp"

namespace MobileRT {
    class Shape {
    public:
        virtual ~Shape() noexcept = default;

        virtual bool intersect(Intersection &/*intersection*/,
                               const Ray &/*ray*/, const Material &/*material*/) const noexcept = 0;

        virtual float getZ() const noexcept = 0;

        virtual void moveTo(float x, float y) noexcept = 0;

        virtual const Point3D getPositionMin() const noexcept = 0;

        virtual const Point3D getPositionMax() const noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_SHAPE_H
