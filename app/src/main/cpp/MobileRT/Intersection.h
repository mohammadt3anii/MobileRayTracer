//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_INTERSECTION_H
#define MOBILERAYTRACER_INTERSECTION_H

#include "myPoint.h"
#include "myVect.h"
#include "myMaterial.h"

namespace MobileRT {
    class Intersection {
    private:

    public:
        bool intersected; // was there an intersection?
        myPoint* p;   // intersection point
        myVect* N;    // intersection normal
        float t;     // ray length parameter
        myMaterial* m;  // material of the intersected primitive

        Intersection ();
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_H