//
// Created by puscas on 06-03-2017.
//

#include "AxisAlignedBoundingBox.h"

using namespace MobileRT;

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Point3D &pointMin, const Point3D &pointMax) :
    pointMin_(pointMin), pointMax_(pointMax)
{
}

bool AxisAlignedBoundingBox::intersect(Intersection &, const Ray &ray,
               const Material &) const
{
    float tmin ((pointMin_.x_ - ray.origin_.x_) / ray.direction_.x_);
    float tmax ((pointMax_.x_ - ray.origin_.x_) / ray.direction_.x_);

    if (tmin > tmax)
        std::swap(tmin, tmax);

    float tymin ((pointMin_.y_ - ray.origin_.y_) / ray.direction_.y_);
    float tymax ((pointMax_.y_ - ray.origin_.y_) / ray.direction_.y_);

    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin ((pointMin_.z_ - ray.origin_.z_) / ray.direction_.z_);
    float tzmax ((pointMax_.z_ - ray.origin_.z_) / ray.direction_.z_);

    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}
