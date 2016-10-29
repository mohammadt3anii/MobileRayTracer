//
// Created by puscas on 16-10-2016.
//

#include "Plane.h"
#include <math.h>

using namespace MobileRT;

 float Plane::compute_d () {
    return -N->not_dot(*P);
}

Plane::Plane (Point* pP, Vect* pN) :
    P(pP), N(pN)
{
    N->normalize();
    d = compute_d ();
}

Intersection* Plane::Intersect (const Ray& r) {
    Point org(*r.orig);

    float N_dir = N->dot(*r.dir);
    // is ray parallel or contained in the Plane ??
    if (fabs(N_dir) < 1e-8f) return new Intersection();  // zero

    // planes have two sides!!!

    float N_O = N->not_dot(org);

    float t = -(d + N_O) / N_dir;

    // is it in front of the eye?
    if (t <= 1.e-6f) {
        return new Intersection();
    }
    //* is it farther than the ray length ??
    if (t >= r.max_T) {
        return new Intersection();
    }

    // if so, then we have an intersection
    Intersection* isect = new Intersection(
        org + ((*r.dir) * t),
        *N,
        t);

    return isect;
}