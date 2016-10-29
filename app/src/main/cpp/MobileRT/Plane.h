//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PLANE_H
#define MOBILERAYTRACER_PLANE_H

#include "Shape.h"

namespace MobileRT {
    class Plane : public Shape {
    private:
        float d;    // 0th degree coefiicient of the plane equation
        float compute_d ();

        const Point point_;   // point in the plane
        const Vect normal_;    // normal to the plane

    public:
        Plane (const Point& point, const Vect& normal);
        virtual Intersection* Intersect (const Ray& ray);
    };
}

#endif //MOBILERAYTRACER_PLANE_H