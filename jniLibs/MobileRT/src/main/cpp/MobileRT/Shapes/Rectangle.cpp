//
// Created by Tiago on 10-Jul-17.
//

#include "Rectangle.hpp"

using MobileRT::Rectangle;
using MobileRT::Point3D;

Rectangle::Rectangle(const Point3D &pointA,
                     const Point3D &pointB,
                     const Point3D &pointC) noexcept :
        pointA_(pointA),
        pointB_(pointB),
		pointC_(pointC),
        pointD_(pointA_ + (pointB - pointA) + (pointC - pointA)),
        AB_(pointB - pointA),
        AC_(pointC - pointA),
        normal_(AB_.crossProduct(AC_))
{
}

bool Rectangle::intersect(Intersection &intersection, const Ray &ray,
                         const Material &material) const noexcept {
    const Vector3D perpendicularVector(ray.direction_, this->AC_);
    const float normalizedProjection(AB_.dotProduct(perpendicularVector));
    if (normalizedProjection * (1.0f + (normalizedProjection < 0.0f) * -2.0f) < VECT_PROJ_MIN) {
		return false;
	}

    const float normalizedProjectionInv(1.0f / normalizedProjection);

    const Vector3D vertexToCamera(ray.origin_, pointA_);

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

void Rectangle::moveTo(float, float) noexcept {
}

float Rectangle::getZ() const noexcept {
    return 0.0f;
}

const Point3D Rectangle::getPositionMin() const noexcept {
    float x, y, z;

    if (pointA_.x_ < pointB_.x_ && pointA_.x_ < pointC_.x_ && pointA_.x_ < pointD_.x_) {
        x = pointA_.x_;
	}
    else if (pointB_.x_ < pointC_.x_ && pointB_.x_ < pointD_.x_) {
        x = pointB_.x_;
	}
    else if (pointC_.x_ < pointD_.x_) {
        x = pointC_.x_;
	}
    else {
        x = pointD_.x_;
	}

    if (pointA_.y_ < pointB_.y_ && pointA_.y_ < pointC_.y_ && pointA_.y_ < pointD_.y_) {
        y = pointA_.y_;
	}
    else if (pointB_.y_ < pointC_.y_ && pointB_.y_ < pointD_.y_) {
        y = pointB_.y_;
	}
    else if (pointC_.y_ < pointD_.y_) {
        y = pointC_.y_;
	}
    else {
        y = pointD_.y_;
	}

    if (pointA_.z_ < pointB_.z_ && pointA_.z_ < pointC_.z_ && pointA_.z_ < pointD_.z_) {
        z = pointA_.z_;
	}
    else if (pointB_.z_ < pointC_.z_ && pointB_.z_ < pointD_.z_) {
        z = pointB_.z_;
	}
    else if (pointC_.z_ < pointD_.z_) {
        z = pointC_.z_;
	}
    else {
        z = pointD_.z_;
	}

    return Point3D(x, y, z);
}

const Point3D Rectangle::getPositionMax() const noexcept {
    float x, y, z;

    if (pointA_.x_ > pointB_.x_ && pointA_.x_ > pointC_.x_ && pointA_.x_ > pointD_.x_) {
        x = pointA_.x_;
	}
    else if (pointB_.x_ > pointC_.x_ && pointB_.x_ > pointD_.x_) {
        x = pointB_.x_;
	}
    else if (pointC_.x_ > pointD_.x_) {
        x = pointC_.x_;
	}
    else {
        x = pointD_.x_;
	}

    if (pointA_.y_ > pointB_.y_ && pointA_.y_ > pointC_.y_ && pointA_.y_ > pointD_.y_) {
        y = pointA_.y_;
	}
    else if (pointB_.y_ > pointC_.y_ && pointB_.y_ > pointD_.y_) {
        y = pointB_.y_;
	}
    else if (pointC_.y_ > pointD_.y_) {
        y = pointC_.y_;
	}
    else {
        y = pointD_.y_;
	}

    if (pointA_.z_ > pointB_.z_ && pointA_.z_ > pointC_.z_ && pointA_.z_ > pointD_.z_) {
        z = pointA_.z_;
	}
    else if (pointB_.z_ > pointC_.z_ && pointB_.z_ > pointD_.z_) {
        z = pointB_.z_;
	}
    else if (pointC_.z_ > pointD_.z_) {
        z = pointC_.z_;
	}
    else {
        z = pointD_.z_;
	}

    return Point3D(x, y, z);
}
