//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PLANE_H
#define MOBILERAYTRACER_PLANE_H


#include "myShape.h"

namespace MobileRT {
    class Plane : public myShape {
    private:
     float d;    // 0th degree coefiicient of the plane equation

    public:
     myPoint* P;   // point in the plane
     myVect* N;    // normal to the plane

     float compute_d ();

        Plane (myPoint* pP, myVect* pN);

     Intersection* Intersect (Ray r);

    };
}

#endif //MOBILERAYTRACER_PLANE_H
