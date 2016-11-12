//
// Created by puscas on 16-10-2016.
//

#include "Plane.hpp"
#include "Constants.hpp"

using namespace MobileRT;

Plane::Plane (const Point& point, const Vect& normal) :
    point_(point),
    normal_(normal.returnNormalized())
{
}

bool Plane::Intersect(const Ray& ray, const Material* material, Intersection& intersection) const
{
    // is ray parallel or contained in the Plane ??
    // planes have two sides!!!
    const float normalized_projection = this->normal_.dot(ray.dir);
    if (normalized_projection >= -MIN_VECT_PROJ && 
        normalized_projection <= MIN_VECT_PROJ) return false;  // zero

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const float distance = this->normal_.dot(this->point_ - ray.orig) / normalized_projection;

    // is it in front of the eye?
    //* is it farther than the ray length ??
    if (distance <= MIN_LENGTH || distance >= ray.maxDistance)
    {
        //return Intersection();
        return false;
    }

    // if so, then we have an intersection
    intersection.setIntersection(
        ray.orig + (ray.dir * distance),
        this->normal_,
        distance,
        material);
    return true;
}