//
// Created by puscas on 16-10-2016.
//

#include "Sphere.hpp"
#include <cmath>

using namespace MobileRT;

Sphere::Quadratic_Sol::Quadratic_Sol () :
    has_sol (false),
    t0 (0.0f),
    t1 (0.0f)
{
}

Sphere::Quadratic_Sol::Quadratic_Sol (const float pt0, const float pt1) :
    has_sol (true),
    t0 (pt0),
    t1 (pt1)
{
}

Sphere::Quadratic_Sol Sphere::Quadratic (const float A, const float B, const float C) const
{
    const float discriminant = B * B - 4.0f * A * C;
    if (discriminant <= 0.0f) return Quadratic_Sol();
    const float rootDiscriminant = std::sqrt(discriminant);

    const float q = (B < 0.0f)? -0.5f * (B - rootDiscriminant) : -0.5f * (B + rootDiscriminant);
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
    center(Point()),// (0,0,0)
    sq_center (Point(this->center.square())),
    radius (1.0f),
    sq_radius (this->radius * this->radius)
{  // unit sphere
}

Sphere::Sphere (const float r) :
    center(Point()),// (0,0,0)
    sq_center (Point(this->center.square())),
    radius(r),
    sq_radius (this->radius * this->radius)
{
}

Sphere::Sphere (const Point& c) :
    center(c),
    sq_center (Point(this->center.square())),
    radius(1.0f),
    sq_radius (this->radius * this->radius)
{
}

Sphere::Sphere (const Point& c, const float r) :
    center(c),
    sq_center (Point(this->center.square())),
    radius(r),
    sq_radius (this->radius * this->radius)
{
}

Intersection* Sphere::Intersect(const Ray &ray, const Material* material, float dist) const
{
    // pull the ray origin a small epsilon along the ray direction
    const Point org(ray.orig + ((ray.dir) * 1.0e-5f));
    const Vect C2O(org - this->center);

    // compute the quadratic equation coefficients
    const float B = 2.0f * C2O.dot(ray.dir);
    const float C = ((org - (this->center * 2.0f)).not_dot(org)) +
        this->sq_center.sumCoordenates() - this->sq_radius;

    // the ray direction is NORMALIZED ( A = 1.0f)
    const Sphere::Quadratic_Sol q_sol = Quadratic(1.0f, B, C);
    if (q_sol.has_sol == false)
    {
        //return Intersection();
        return nullptr;
    }

    const float t0 = q_sol.t0;
    const float t1 = q_sol.t1;

    if (t0 > ray.max_T || t1 < 1.0e-6f)
    {
        return nullptr;
    }

    float t = t0;
    if (t < 1.0e-6f)
    {
        t = t1;
        if (t > ray.max_T)
        {
            return nullptr;
        }
    }

    if(t >= dist) return nullptr;

    const Point point(org + ((ray.dir) * t));
    Vect normal(point - this->center);
    // if the length of the C2O vector is less that radius then the ray origin is inside the sphere
    //if (C2O.length() < radius) isect->N.mult(-1.0f);
    normal.normalize();

    // if so, then we have an intersection
    return new Intersection(
            point,
            normal,
            t,
            material);
}