//
// Created by puscas on 16-10-2016.
//

#include "Sphere.h"
#include <cmath>



using namespace MobileRT;

void Sphere::square_params ()
{
    sq_radius = radius * radius;
    sq_center = new myPoint(center->x * center->x, center->y * center->y, center->z * center->z);
}

Sphere::Quadratic_Sol* Sphere::Quadratic (float A, float B, float C)
{
    float t0, t1;
    float discrim = B * B - 4.f * A * C;
    if (discrim <= 0.) return new Quadratic_Sol();
    float rootDiscrim = (float) std::sqrt((double)discrim);

    float q;
    if (B < 0.f) q = -.5f * (B - rootDiscrim);
    else q = -.5f * (B + rootDiscrim);
    t0 = q / A;
    t1 = C / q;
    if (t0 > t1) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
    return new Quadratic_Sol(t0, t1);
}

Sphere::Sphere ()
{  // unit sphere
    center = new myPoint(); // (0,0,0)
    radius = 1.f;
    square_params();
}

Sphere::Sphere (float r)
{
    center = new myPoint();
    radius = r;
    square_params();
}

Sphere::Sphere (myPoint* c)
{
center = c;
radius = 1.f;
square_params();
}

Sphere::Sphere (myPoint* c, float r)
{
center = c;
radius = r;
square_params();
}


Intersection* Sphere::Intersect (const Ray& r)
{
    // pull the ray origin a small epsilon along the ray direction
    myPoint* org = new myPoint(*r.orig);
    org->x += r.dir->x * 1e-5f;
    org->y += r.dir->y * 1e-5f;
    org->z += r.dir->z * 1e-5f;
    myVect* C2O = new myVect (org->x-center->x, org->y - center->y, org->z - center->z);

    // compute the quadratic equation coefficients
    float A, B, C;

    A = 1.f;  // the ray direction is NORMALIZED
    B = 2.f * (C2O->x* r.dir->x + C2O->y * r.dir->y + C2O->z * r.dir->z);
    C = (org->x - 2.f*center->x) * org->x + (org->y - 2.f*center->y) * org->y + (org->z - 2.f*center->z) * org->z;
    C += sq_center->x + sq_center->y + sq_center->z - sq_radius;

    Sphere::Quadratic_Sol* q_sol = Quadratic(A, B, C);
    if (!q_sol->has_sol)
        return new Intersection();

    float t0=q_sol->t0, t1=q_sol->t1;

    if (t0 > r.max_T || t1 < 1e-6f)
        return new Intersection();
    float t = t0;
    if (t < 1e-6f) {
        t = t1;
        if (t > r.max_T) return new Intersection();
    }

    // if so, then we have an intersection
    Intersection* isect = new Intersection();

    isect->intersected = true;

    isect->t = t;

    isect->p->x = org->x + t * r.dir->x;
    isect->p->y = org->y + t * r.dir->y;
    isect->p->z = org->z + t * r.dir->z;

    isect->N->x = isect->p->x - center->x;
    isect->N->y = isect->p->y - center->y;
    isect->N->z = isect->p->z - center->z;

    // if the length of the C2O vector is less that radius then the ray origin is inside the sphere
    //if (C2O.length() < radius) isect->N.mult(-1.f);
    isect->N->normalize();

    return isect;
}


