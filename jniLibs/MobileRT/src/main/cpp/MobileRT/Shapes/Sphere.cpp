//
// Created by Tiago on 16-10-2016.
//

#include "Sphere.hpp"

using MobileRT::Sphere;
using MobileRT::Point3D;

Sphere::Sphere(const Point3D &center, const float radius) noexcept :
        sq_radius_(radius * radius),
        center_(center)
{
}

bool Sphere::intersect(Intersection &intersection, const Ray &ray,
                       const Material &material) const noexcept {
//stackoverflow.com/questions/1986378/how-to-set-up-quadratic-equation-for-a-ray-sphere-intersection
    const Vector3D centerToOrigin(ray.origin_, this->center_);

    //A = 1.0 - normalized vectors
    const float B(2.0f * centerToOrigin.dotProduct(ray.direction_));
    const float C(centerToOrigin.squareMagnitude() - this->sq_radius_);

    const float discriminant(B * B - 4.0f * C);
    //don't intersect (ignores tangent point of the sphere)
    if (discriminant <= 0.0f) {return false;}

    //ray intersects the sphere in 2 points
    const float rootDiscriminant(std::sqrt(discriminant));
    const float distanceToIntersection1((-B + rootDiscriminant) * 0.5f);
    const float distanceToIntersection2((-B - rootDiscriminant) * 0.5f);
    //distance between intersection and camera = smaller root = closer intersection
    const float distanceToIntersection(
		distanceToIntersection1 < distanceToIntersection2 ?
		distanceToIntersection1 : distanceToIntersection2);

    if (distanceToIntersection < RAY_LENGTH_MIN || distanceToIntersection > intersection.length_) {
        return false;
	}

    // if so, then we have an intersection
    intersection.reset(ray.origin_, ray.direction_, distanceToIntersection,
                       this->center_,
                       material);

    return true;
}

void Sphere::moveTo(const float x, const float y) noexcept {
    this->center_.x_ = x;
    this->center_.y_ = y;
}

float Sphere::getZ() const noexcept {
    return this->center_.z_;
}

Point3D Sphere::getPositionMin() const noexcept {
    float x, y, z;
    const float radius(std::sqrt(this->sq_radius_));

    x = this->center_.x_ - radius;
    y = this->center_.x_ - radius;
    z = this->center_.x_ - radius;

    return Point3D(x, y, z);
}

Point3D Sphere::getPositionMax() const noexcept {
    float x, y, z;
    const float radius(std::sqrt(this->sq_radius_));

    x = this->center_.x_ + radius;
    y = this->center_.x_ + radius;
    z = this->center_.x_ + radius;

    return Point3D(x, y, z);
}
