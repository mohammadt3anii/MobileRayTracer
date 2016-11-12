//
// Created by puscas on 16-10-2016.
//

#include "Sphere.h"
#include <cmath>
#include <algorithm>

using namespace MobileRT;

Sphere::Sphere (const Point& center, const float radius) :
    center_(center),
    sq_radius_(radius*radius)
{
}

bool Sphere::Intersect(const Ray& ray, const Material* material, Intersection& intersection) const
{
    //http://stackoverflow.com/questions/1986378/how-to-set-up-quadratic-equation-for-a-ray-sphere-intersection
    const Vect centerToOrigin(ray.orig - this->center_);

    //A = 1.0
    const float B = 2.0f * centerToOrigin.dot(ray.dir);
    const float C = ((centerToOrigin.x*centerToOrigin.x) + (centerToOrigin.y*centerToOrigin.y) + (centerToOrigin.z*centerToOrigin.z) - this->sq_radius_);
    
    const float discriminant = B * B - 4.0f * C;
    if (discriminant <= 0.0f) return false;

    const float rootDiscriminant = std::sqrt(discriminant);
    const float t0 = (-B + rootDiscriminant) * 0.5f;
    const float t1 = (-B - rootDiscriminant) * 0.5f;
    const float distance = std::min(t0, t1);

    if (distance > ray.maxDistance || distance < 1.0e-6f) return false;

    const Point point(ray.orig + ((ray.dir) * distance));
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