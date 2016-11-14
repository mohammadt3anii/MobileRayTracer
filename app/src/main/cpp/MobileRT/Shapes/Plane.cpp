//
// Created by Tiago on 16-10-2016.
//

#include "Plane.h"
#include "../Constants.h"

using namespace MobileRT;

Plane::Plane (const Point3D& point, const Vector3D& normal) :
    point_(point),
    normal_(normal.returnNormalized())
{
}

bool Plane::intersect(const Ray& ray, const Material* material, Intersection& intersection) const
{
    // is ray parallel or contained in the Plane ??
    // planes have two sides!!!
    const float normalized_projection (this->normal_.dotProduct(ray.direction_));
    if (normalized_projection >= -MIN_VECT_PROJ && 
        normalized_projection <= MIN_VECT_PROJ) return false;  // zero

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const float distance (this->normal_.dotProduct(this->point_ - ray.origin_) / normalized_projection);

    // is it in front of the eye?
    //* is it farther than the ray length ??
    if (distance <= MIN_LENGTH || distance >= ray.maxDistance_)
    {
        //return Intersection();
        return false;
    }

    // if so, then we have an intersection
    intersection.setIntersection(
            ray.origin_ + (ray.direction_ * distance),
            this->normal_,
            distance,
            material);

    return true;
}