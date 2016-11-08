//
// Created by puscas on 16-10-2016.
//

#include "Sphere.h"
#include <cmath>

using namespace MobileRT;

void Sphere::square_params ()
{
    this->sq_radius = radius * radius;
    this->sq_center = new Point(center->square());
}

Sphere::Quadratic_Sol Sphere::Quadratic (const float A, const float B, const float C) const
{
    float discrim = B * B - 4.0f * A * C;
    if (discrim <= 0.) return Quadratic_Sol();
    float rootDiscrim = (float) std::sqrt(discrim);

    float q;
    if (B < 0.0f) q = -.5f * (B - rootDiscrim);
    else q = -.5f * (B + rootDiscrim);
    float t0 = q / A;
    float t1 = C / q;
    if (t0 > t1) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
    return Quadratic_Sol(t0, t1);
}

Sphere::Sphere () :
    center(new Point()),// (0,0,0)
    radius(1.0f)
{  // unit sphere
    square_params();
}

Sphere::Sphere (const float r) :
    center(new Point()),// (0,0,0)
    radius(r)
{
    square_params();
}

Sphere::Sphere (Point* c) :
    center(c),
    radius(1.0f)
{
    square_params();
}

Sphere::Sphere (Point* c, const float r) :
    center(c),
    radius(r)
{
    square_params();
}

Intersection Sphere::Intersect(const Ray &ray) const
{
    // pull the ray origin a small epsilon along the ray direction
    Point org(ray.orig + ((ray.dir) * 1e-5f));
    Vect C2O(org - *this->center);

    // compute the quadratic equation coefficients
    float B = 2.0f * C2O.dot(ray.dir);
    float C = (org - (*this->center * 2.0f)).not_dot(org);
    C += this->sq_center->sumCoordenates() - this->sq_radius;

    // the ray direction is NORMALIZED ( A = 1.0f)
    Sphere::Quadratic_Sol q_sol = Quadratic(1.0f, B, C);
    if (!q_sol.has_sol)
    {
        return Intersection();
    }

    float t0 = q_sol.t0;
    float t1 = q_sol.t1;

    if (t0 > ray.max_T || t1 < 1e-6f)
    {
        return Intersection();
    }

    float t = t0;
    if (t < 1e-6f)
    {
        t = t1;
        if (t > ray.max_T)
        {
            return Intersection();
        }
    }

    Point point(org + ((ray.dir) * t));
    Vect normal(point - *this->center);
    // if the length of the C2O vector is less that radius then the ray origin is inside the sphere
    //if (C2O.length() < radius) isect->N.mult(-1.0f);
    normal.normalize();

    // if so, then we have an intersection
    return Intersection(
            point,
            normal,
            t);
}