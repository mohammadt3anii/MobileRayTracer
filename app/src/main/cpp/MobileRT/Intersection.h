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

/*
package com.example.lps.mobilert;

public final class Intersection {
public boolean intersected; // was there an intersection?
public myPoint p;   // intersection point
public myVect N;    // intersection normal
public float t;     // ray length parameter
public myMaterial m;  // material of the intersected primitive

    Intersection () {  // constructor for no intersection
        intersected = false;
        t = 1E10f;      // this is the max-T used through out this implementation
        p = new myPoint();
        N = new myVect();
        m = new myMaterial();
    }

}
*/