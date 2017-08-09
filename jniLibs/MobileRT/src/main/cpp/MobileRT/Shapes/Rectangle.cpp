//
// Created by Tiago on 10-Jul-17.
//

#include "Rectangle.hpp"

using MobileRT::AABB;
using MobileRT::Rectangle;
using MobileRT::Point3D;

Rectangle::Rectangle (Point3D pointA,
                      Point3D pointB,
                      Point3D pointC) noexcept :
  AC_ {pointC - pointA},
  AB_ {pointB - pointA},
  normal_ {AB_ . crossProduct (AC_)},
  pointA_ {pointA},
  pointB_ {pointB},
  pointC_ {pointC},
  pointD_ {pointA_ + (pointB - pointA) + (pointC - pointA)}
{
}

bool Rectangle::intersect (Intersection *const intersection, Ray ray) const noexcept {

  const Vector3D perpendicularVector {ray . direction_, this -> AC_};
  const float normalizedProjection {this -> AB_ . dotProduct (perpendicularVector)};
  if (std::fabs (normalizedProjection) < Epsilon) {
    return false;
  }

  const float normalizedProjectionInv {1.0f / normalizedProjection};
  const Vector3D vertexToCamera {ray . origin_, this -> pointA_};
  const float u {normalizedProjectionInv * vertexToCamera . dotProduct (perpendicularVector)};

    if (u < 0.0f || u > 1.0f) {
      return false;
    }

  const Vector3D upPerpendicularVector {vertexToCamera, this -> AB_};//cross product
  const float v {normalizedProjectionInv * ray . direction_ . dotProduct (upPerpendicularVector)};

    if (v < 0.0f || (u + v) > 1.0f) {
      return false;
    }

    // at this stage we can compute t to find out where
    // the intersection point is on the line
  const float distanceToIntersection {
    normalizedProjectionInv * this -> AC_ . dotProduct (upPerpendicularVector)};

  if (distanceToIntersection < Epsilon || distanceToIntersection > intersection -> length_) {
    return false;
  }

    intersection->reset(
            ray.origin_, ray.direction_, distanceToIntersection,
            this->normal_);

    return true;
}

void Rectangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Rectangle::getZ() const noexcept {
    return 0.0f;
}

Point3D Rectangle::getPositionMin() const noexcept {
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

Point3D Rectangle::getPositionMax() const noexcept {
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

AABB Rectangle::getAABB() const noexcept {
	return AABB(getPositionMin(), getPositionMax());
}

bool Rectangle::intersect (AABB /*box*/) const noexcept {

  return false;
}
