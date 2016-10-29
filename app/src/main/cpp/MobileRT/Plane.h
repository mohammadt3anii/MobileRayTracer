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

    public:
        Point* P;   // point in the plane
        Vect* N;    // normal to the plane


        Plane (Point* pP, Vect* pN);
        virtual Intersection* Intersect (const Ray& r);
    };
}

#endif //MOBILERAYTRACER_PLANE_H