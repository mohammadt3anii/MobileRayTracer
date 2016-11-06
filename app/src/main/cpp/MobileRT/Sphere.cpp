//
// Created by puscas on 16-10-2016.
//

#include "Sphere.h"
#include <cmath>



using namespace MobileRT;

void Sphere::square_params () {
    sq_radius = radius * radius;
    sq_center = new Point(center->square());
}

Sphere::Quadratic_Sol* Sphere::Quadratic (float A, float B, float C) {
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

Sphere::Sphere () {  // unit sphere
    center = new Point(); // (0,0,0)
    radius = 1.f;
    square_params();
}

Sphere::Sphere (float r) {
    center = new Point();
    radius = r;
    square_params();
}

Sphere::Sphere (Point* c) {
center = c;
radius = 1.f;
square_params();
}

Sphere::Sphere (Point* c, float r) {
center = c;
radius = r;
square_params();
}


Intersection Sphere::Intersect(const Ray &ray) {
    // pull the ray origin a small epsilon along the ray direction
    Point org(ray.orig + ((ray.dir) * 1e-5f));
    Vect C2O(org - *center);

    // compute the quadratic equation coefficients
    float B, C;

    B = 2.f * C2O.dot(ray.dir);
    C = (org - (*center * 2.f)).not_dot(org);
    C += sq_center->sumCoordenates() - sq_radius;

    // the ray direction is NORMALIZED ( A = 1.0f)
    Sphere::Quadratic_Sol* q_sol = Quadratic(1.f, B, C);
    if (!q_sol->has_sol) {
        return Intersection();
    }

    float t0=q_sol->t0, t1=q_sol->t1;

    if (t0 > ray.max_T || t1 < 1e-6f) {
        return Intersection();
    }

    float t = t0;
    if (t < 1e-6f) {
        t = t1;
        if (t > ray.max_T) {
            return Intersection();
        }
    }

    Point point(org + ((ray.dir) * t));
    Vect normal(point - *center);
    // if the length of the C2O vector is less that radius then the ray origin is inside the sphere
    //if (C2O.length() < radius) isect->N.mult(-1.f);
    normal.normalize();

    // if so, then we have an intersection
    return Intersection(
            point,
            normal,
            t);
}