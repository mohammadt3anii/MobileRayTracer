//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PLANE_H
#define MOBILERAYTRACER_PLANE_H

#include "Shape.h"

namespace MobileRT
{
    class Plane : public Shape
    {
        private:
            const Point point_;   // point in the plane
            const Vect normal_;    // normal to the plane
            const float d_;    // 0th degree coefiicient of the plane equation

        public:
            Plane (const Point& point, const Vect& normal);

            Intersection Intersect(const Ray &ray) const override;
    };
}

#endif //MOBILERAYTRACER_PLANE_H