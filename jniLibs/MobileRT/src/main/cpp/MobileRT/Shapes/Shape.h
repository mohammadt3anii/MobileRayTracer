//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHAPE_H
#define MOBILERAYTRACER_SHAPE_H

#include "../Utils.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Material.h"

namespace MobileRT {
    class Shape {
    public:
        virtual ~Shape(void) = default;

        virtual bool intersect(Intersection &, const Ray &, const Material &) const = 0;

        virtual float getZ(void) const;
        virtual void moveTo(float x, float y);
        virtual float getMaxDistance(Point3D point) const;
    };
}

#endif //MOBILERAYTRACER_SHAPE_H
