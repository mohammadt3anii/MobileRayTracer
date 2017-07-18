//
// Created by Tiago on 16-10-2016.
//

#include "Plane.hpp"

using MobileRT::Plane;
using MobileRT::Point3D;

Plane::Plane(const Point3D &point, const Vector3D &normal) noexcept :
        point_(point),
        normal_(normal)
{
}

bool Plane::intersect(Intersection &intersection, const Ray &ray,
                      const Material &material) const noexcept {
    // is ray parallel or contained in the Plane ??
    // planes have two sides!!!
    const float normalized_projection(this->normal_.dotProduct(ray.direction_));
    if (std::fabs(normalized_projection) < VECT_PROJ_MIN) {
        return false;
	}

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const float distanceToIntersection(
            this->normal_.dotProduct(this->point_, ray.origin_) / normalized_projection);

    // is it in front of the eye?
    // is it farther than the ray length ??
    if (distanceToIntersection < RAY_LENGTH_MIN || distanceToIntersection > intersection.length_) {
        return false;
	}

    // if so, then we have an intersection
    intersection.reset(ray.origin_, ray.direction_, distanceToIntersection,
                       this->normal_,
                       material);

    return true;
}

void Plane::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Plane::getZ() const noexcept {
    return 0.0f;
}

Point3D Plane::getPositionMin() const noexcept {
    return this->point_;
}

Point3D Plane::getPositionMax() const noexcept {
    return this->point_;
}
