//
// Created by Tiago on 16-10-2016.
//

#include "Sphere.h"
#include "Constants.h"
#include <cmath>

using namespace MobileRT;

Sphere::Sphere (const Point& center, const float radius) :
    center_(center),
    sq_radius_(radius*radius)
{
}

bool Sphere::Intersect(const Ray& ray, const Material* material, Intersection& intersection) const
{
    //http://stackoverflow.com/questions/1986378/how-to-set-up-quadratic-equation-for-a-ray-sphere-intersection
    const Vect centerToOrigin(ray.origin_ - this->center_);

    //A = 1.0
    const float B (2.0f * centerToOrigin.dot(ray.direction_));
    const float C (centerToOrigin.dot() - this->sq_radius_);
    
    const float discriminant (B * B - 4.0f * C);
    if (discriminant <= 0.0f) return false;

    const float rootDiscriminant (std::sqrt(discriminant));
    const float t0 ((-B + rootDiscriminant) * 0.5f);
    const float t1 ((-B - rootDiscriminant) * 0.5f);
    const float distance ((t0 < t1) ? t0 : t1);

    if (distance > ray.maxDistance_ || distance < MIN_RAY_DIST) return false;

    const Point point(ray.origin_ + (ray.direction_ * distance));
    Vect normal(point - this->center_);
    normal.normalize();

    // if so, then we have an intersection
    intersection.setIntersection(
        point,
        normal,
        distance,
        material);

    return true;
}