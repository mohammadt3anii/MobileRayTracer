//
// Created by Tiago on 16-10-2016.
//

#include "Plane.hpp"

using MobileRT::AABB;
using MobileRT::Plane;
using MobileRT::Point3D;
using MobileRT::Vector3D;

Plane::Plane (const Point3D point, const Vector3D normal) noexcept :
  normal_ {normal.returnNormalized()},
  point_ {point}
{
}

bool Plane::intersect (Intersection *const intersection, const Ray ray) const noexcept {
    // is ray parallel or contained in the Plane ??
    // planes have two sides!!!
  const float normalized_projection {this->normal_.dotProduct (ray.direction_)};
  if (std::fabs (normalized_projection) < Epsilon) {
    return false;
  }

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
  const float distanceToIntersection {
    this->normal_.dotProduct (this->point_, ray.origin_) / normalized_projection};

  // is it in front of the eye?
  // is it farther than the ray length ??
  if (distanceToIntersection < Epsilon || distanceToIntersection > intersection->length_) {
        return false;
	}

  // if so, then we have an intersection
  intersection->reset (ray.origin_, ray.direction_, distanceToIntersection,
                       this->normal_);
  return true;
}

void Plane::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Plane::getZ() const noexcept {
    return 0.0f;
}

Vector3D Plane::getRightVector () const noexcept {
    //const Vector3D up {0, 1, 0};
  Vector3D right {};
  if (this->normal_.x_ >= 1) {
    right = Vector3D {0, 1, 1};
  } else if (this->normal_.y_ >= 1) {
    right = Vector3D {1, 0, 1};
  } else if (this->normal_.z_ >= 1) {
    right = Vector3D {1, 1, 0};
  } else if (this->normal_.x_ <= -1) {
    right = Vector3D {0, 1, 1};
  } else if (this->normal_.y_ <= -1) {
    right = Vector3D {1, 0, 1};
  } else if (this->normal_.z_ <= -1) {
    right = Vector3D {1, 1, 0};
  }
  right.normalize ();
  const float cosAngle {
    this->normal_.dotProduct (right) / (this->normal_.magnitude_ * right.magnitude_)};
  if (cosAngle != 0) {
    LOG("MAL");
  }
  return right;
}

Point3D Plane::getPositionMin () const noexcept {
  return this->point_ + getRightVector () * -100.0f;
}

Point3D Plane::getPositionMax() const noexcept {
  return this->point_ + getRightVector () * 100.0f;
}

AABB Plane::getAABB() const noexcept {
  const AABB box {getPositionMin (), getPositionMax ()};
  return box;
}

float Plane::distance (const Point3D point) const noexcept {
  //Plane Equation
  //a(x-x0)+b(y-y0)+c(z-z0) = 0
  //abc = normal
  //x0,y0,z0 = point
  //D = |ax0 + by0 + cz0 + d| / sqrt(a² + b² + c²)
  const float d {
    normal_.x_ * - point_.x_ + normal_.y_ * - point_.y_ + normal_.z_ * - point_.z_};
  const float numerator {
    normal_.x_ * point.x_ + normal_.y_ * point.y_ + normal_.z_ * point.z_ + d};
  const float denumerator {
    std::sqrt (
      normal_.x_ * normal_.x_ + normal_.y_ * normal_.y_ + normal_.z_ * normal_.z_)};
  return numerator / denumerator;
}

bool Plane::intersect (const AABB box) const noexcept {
  /*Point3D positiveVertex {box.pointMin_};
  Point3D negativeVertex {box.pointMax_};*/
  Point3D positiveVertex {box.pointMax_};
  Point3D negativeVertex {box.pointMin_};

  /*if (this->normal_.x_ >= 0.0f) {
    positiveVertex.x_ = box.pointMax_.x_;
    negativeVertex.x_ = box.pointMin_.x_;
  } else if (this->normal_.y_ >= 0.0f) {
    positiveVertex.y_ = box.pointMax_.y_;
    negativeVertex.y_ = box.pointMin_.y_;
  } else if (this->normal_.z_ >= 0.0f) {
    positiveVertex.z_ = box.pointMax_.z_;
    negativeVertex.z_ = box.pointMin_.z_;
  } else if (this->normal_.x_ < 0.0f) {
    positiveVertex.x_ = box.pointMin_.x_;
    negativeVertex.x_ = box.pointMax_.x_;
  } else if (this->normal_.y_ < 0.0f) {
    positiveVertex.y_ = box.pointMin_.y_;
    negativeVertex.y_ = box.pointMax_.y_;
  } else if (this->normal_.z_ < 0.0f) {
    positiveVertex.z_ = box.pointMin_.z_;
    negativeVertex.z_ = box.pointMax_.z_;
  }*/

  const float distanceP {distance (positiveVertex)};
  const float distanceN {distance (negativeVertex)};
    const bool res{(distanceP <= 0 && distanceN >= 0) || (distanceP >= 0 && distanceN <= 0)};

  return res;
}
