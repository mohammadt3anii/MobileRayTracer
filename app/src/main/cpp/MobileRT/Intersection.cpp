//
// Created by puscas on 16-10-2016.
//

#include "Intersection.h"

using namespace MobileRT;

Intersection::Intersection () {  // constructor for no intersection
    intersected = false;
    t = 1E10f;      // this is the max-T used through out this implementation
    p = new myPoint();
    N = new myVect();
    m = new myMaterial();
}