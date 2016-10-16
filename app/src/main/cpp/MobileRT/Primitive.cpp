//
// Created by puscas on 16-10-2016.
//

#include "Primitive.h"

using namespace MobileRT;

Primitive::Primitive (myShape* ps, myMaterial* pm) {
    shape=ps; material=pm;
}

Intersection* Primitive::Intersect (Ray r) {
    Intersection* isect = Primitive::shape->Intersect(r);
    if (isect->intersected) isect->m = Primitive::material;

    return isect;
}