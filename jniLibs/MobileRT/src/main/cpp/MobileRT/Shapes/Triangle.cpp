//
// Created by Tiago on 15-11-2016.
// Uses: Moller and Trumbore
//

#include "Triangle.hpp"

using MobileRT::Triangle;
using MobileRT::Point3D;

Triangle::Triangle(const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) noexcept :
        AC_(pointC - pointA),
        AB_(pointB - pointA),
        normal_(AB_.crossProduct(AC_)),
        pointA_(pointA),
        pointB_(pointB),
        pointC_(pointC)
{
}

bool Triangle::intersect(Intersection &intersection, const Ray &ray,
                         const Material &material) const noexcept {
    const Vector3D perpendicularVector(ray.direction_, this->AC_);
    const float normalizedProjection(this->AB_.dotProduct(perpendicularVector));
    if (std::fabs(normalizedProjection) < VECT_PROJ_MIN) {
        return false;
	}

    const float normalizedProjectionInv(1.0f / normalizedProjection);

    const Vector3D vertexToCamera(ray.origin_, this->pointA_);

    const float u(normalizedProjectionInv * vertexToCamera.dotProduct(perpendicularVector));

    if (u < 0.0f || u > 1.0f) {
        return false;
	}

    const Vector3D upPerpendicularVector(vertexToCamera, this->AB_);//cross product
    const float v(normalizedProjectionInv * ray.direction_.dotProduct(upPerpendicularVector));

    if (v < 0.0f || (u + v) > 1.0f) {
        return false;
	}

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    const float distanceToIntersection(
            normalizedProjectionInv * this->AC_.dotProduct(upPerpendicularVector));

    if (distanceToIntersection < RAY_LENGTH_MIN || distanceToIntersection > intersection.length_) {
        return false;
	}

    intersection.reset(
            ray.origin_, ray.direction_, distanceToIntersection,
            this->normal_,
            material);

    return true;
}

void Triangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Triangle::getZ() const noexcept {
    return 0.0f;
}

Point3D Triangle::getPositionMin() const noexcept {
    float x, y, z;

    if (pointA_.x_ < pointB_.x_ && pointA_.x_ < pointC_.x_) {
        x = pointA_.x_;
	}
    else if (pointB_.x_ < pointC_.x_) {
        x = pointB_.x_;
	}
    else {
        x = pointC_.x_;
	}

    if (pointA_.y_ < pointB_.y_ && pointA_.y_ < pointC_.y_) {
        y = pointA_.y_;
	}
    else if (pointB_.y_ < pointC_.y_) {
        y = pointB_.y_;
	}
    else {
        y = pointC_.y_;
	}

    if (pointA_.z_ < pointB_.z_ && pointA_.z_ < pointC_.z_) {
        z = pointA_.z_;
	}
    else if (pointB_.z_ < pointC_.z_) {
        z = pointB_.z_;
	}
    else {
        z = pointC_.z_;
	}

    return Point3D(x, y, z);
}

Point3D Triangle::getPositionMax() const noexcept {
    float x, y, z;

    if (pointA_.x_ > pointB_.x_ && pointA_.x_ > pointC_.x_) {
        x = pointA_.x_;
	}
    else if (pointB_.x_ > pointC_.x_) {
        x = pointB_.x_;
	}
    else {
        x = pointC_.x_;
	}

    if (pointA_.y_ > pointB_.y_ && pointA_.y_ > pointC_.y_) {
        y = pointA_.y_;
	}
    else if (pointB_.y_ > pointC_.y_) {
        y = pointB_.y_;
	}
    else {
        y = pointC_.y_;
	}

    if (pointA_.z_ > pointB_.z_ && pointA_.z_ > pointC_.z_) {
        z = pointA_.z_;
	}
    else if (pointB_.z_ > pointC_.z_) {
        z = pointB_.z_;
	}
    else {
        z = pointC_.z_;
	}

    return Point3D(x, y, z);
}
