//
// Created by puscas on 16-10-2016.
//

#include "Plane.hpp"
#include "Constants.hpp"

using namespace MobileRT;

Plane::Plane (const Point& point, const Vect& normal) :
    point_(point),
    normal_(normal.returnNormalized()),
    d_(-(this->normal_.not_dot(this->point_)))
{
}

bool Plane::Intersect(const Ray& ray, const Material* material, Intersection& intersection)
{
    const float N_dir = this->normal_.dot(ray.dir);
    // is ray parallel or contained in the Plane ??
    if (((N_dir >= 0)? N_dir : -N_dir) < 1e-8f) return false;  // zero

    // planes have two sides!!!

    const float N_O = this->normal_.not_dot(ray.orig);

    const float t = -(this->d_ + N_O) / N_dir;

    // is it in front of the eye?
    //* is it farther than the ray length ??
    if (t <= MIN_LENGTH || t >= ray.max_T)
    {
        //return Intersection();
        return false;
    }

    // if so, then we have an intersection
    intersection.setIntersection(
        ray.orig + (ray.dir * t),
        this->normal_,
        t,
        material);
    return true;
    // return new Intersection(
    //     ray.orig + (ray.dir * t),
    //     this->normal_,
    //     t,
    //     material);
}

bool Plane::Intersect(const Ray& ray, const Material* material, const float maxRayDist, Intersection& intersection)
{
    const float N_dir = this->normal_.dot(ray.dir);
    // is ray parallel or contained in the Plane ??
    if (((N_dir >= 0)? N_dir : -N_dir) < 1e-8f) return false;  // zero

    // planes have two sides!!!

    const float N_O = this->normal_.not_dot(ray.orig);

    const float t = -(this->d_ + N_O) / N_dir;

    // is it in front of the eye?
    //* is it farther than the ray length ??
    if (t <= MIN_LENGTH || t >= ray.max_T || t >= maxRayDist)
    {
        //return Intersection();
        return false;
    }

    // if so, then we have an intersection
    intersection.setIntersection(
        ray.orig + (ray.dir * t),
        this->normal_,
        t,
        material);
    return true;
    // return new Intersection(
    //     ray.orig + (ray.dir * t),
    //     this->normal_,
    //     t,
    //     material);
}