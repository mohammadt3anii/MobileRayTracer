//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHAPE_H
#define MOBILERAYTRACER_SHAPE_H

#include "../Intersection.h"
#include "../Ray.h"
#include "../Material.h"

namespace MobileRT {
    class Shape {
    public:
        virtual ~Shape(void) noexcept;

        virtual bool intersect(Intersection &,
                               const Ray &, const Material &) const noexcept = 0;

        virtual float getZ(void) const noexcept = 0;

        virtual void moveTo(const float x, const float y) noexcept = 0;

        virtual const Point3D getPositionMin(void) const noexcept = 0;

        virtual const Point3D getPositionMax(void) const noexcept = 0;
    };
}

#endif //MOBILERAYTRACER_SHAPE_H
