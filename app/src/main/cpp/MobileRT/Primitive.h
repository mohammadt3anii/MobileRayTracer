//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PRIMITIVE_H
#define MOBILERAYTRACER_PRIMITIVE_H

#include "myShape.h"
#include "myMaterial.h"
#include "Ray.h"

namespace MobileRT {
    class Primitive {
    private:
    public:
     myShape *shape;
     myMaterial *material;
        Primitive (myShape* ps, myMaterial* pm);

     Intersection* Intersect (Ray r);

    };
}

#endif //MOBILERAYTRACER_PRIMITIVE_H


/*
package com.example.lps.mobilert;

public class Primitive {
public myShape shape;
public myMaterial material;

    Primitive (myShape ps, myMaterial pm) {
        shape=ps; material=pm;
    }

public Intersection Intersect (Ray r) {
        Intersection isect = shape.Intersect(r);
        if (isect.intersected) isect.m = material;

        return isect;
    }
}
*/