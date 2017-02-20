//
// Created by Tiago on 16-10-2016.
//

#include "Plane.h"

using namespace MobileRT;

Plane::Plane(const Point3D &point, const Vector3D &normal) :
        point_(point),
        normal_(normal.returnNormalized()) {
}

bool Plane::intersect(Intersection &intersection, const Ray &ray, const Material &material) {
    // is ray parallel or contained in the Plane ??
    // planes have two sides!!!
    const float normalized_projection(this->normal_.dotProduct(ray.direction_));
    const float abs(normalized_projection * (1 + (normalized_projection < 0) * -2));
    if (abs < VECT_PROJ_MIN) return false;

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const float distance(
            this->normal_.dotProduct(this->point_, ray.origin_) / normalized_projection);

    // is it in front of the eye?
    //* is it farther than the ray length ??
    if (distance < RAY_LENGTH_MIN || distance > ray.maxDistance_) {
        return false;
    }

    // if so, then we have an intersection
    intersection.reset(ray.origin_, ray.direction_, distance,
                       this->normal_,
                       distance,
                       material);

    return true;
}
