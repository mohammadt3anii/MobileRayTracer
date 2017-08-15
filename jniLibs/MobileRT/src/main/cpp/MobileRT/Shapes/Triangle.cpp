//
// Created by Tiago on 15-11-2016.
// Uses: Moller and Trumbore
//

#include "Triangle.hpp"

using MobileRT::AABB;
using MobileRT::Triangle;
using MobileRT::Point3D;

Triangle::Triangle (const Point3D pointA, const Point3D pointB, const Point3D pointC) noexcept :
  AC_ {pointC - pointA},
  AB_ {pointB - pointA},
  BC_ {pointC - pointB},
  normal_ {AB_ . crossProduct (AC_)},
  pointA_ {pointA},
  pointB_ {pointB},
  pointC_ {pointC}
{
}

bool Triangle::intersect (Intersection *const intersection, const Ray ray) const noexcept {
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
  intersection->reset (ray.origin_, ray.direction_, distanceToIntersection, this->normal_);
  return true;
}

void Triangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Triangle::getZ() const noexcept {
    return 0.0f;
}

Point3D Triangle::getPositionMin() const noexcept {
  const float x {pointA_.x_ < pointB_.x_ && pointA_.x_ < pointC_.x_ ? pointA_.x_ :
                 pointB_.x_ < pointC_.x_ ? pointB_.x_ :
                 pointC_.x_};
  const float y {pointA_.y_ < pointB_.y_ && pointA_.y_ < pointC_.y_ ? pointA_.y_ :
                 pointB_.y_ < pointC_.y_ ? pointB_.y_ :
                 pointC_.y_};
  const float z {pointA_.z_ < pointB_.z_ && pointA_.z_ < pointC_.z_ ? pointA_.z_ :
                 pointB_.z_ < pointC_.z_ ? pointB_.z_ :
                 pointC_.z_};
  return Point3D (x, y, z);
}

Point3D Triangle::getPositionMax() const noexcept {
  const float x {pointA_.x_ > pointB_.x_ && pointA_.x_ > pointC_.x_ ? pointA_.x_ :
                 pointB_.x_ > pointC_.x_ ? pointB_.x_ : pointC_.x_};
  const float y {pointA_.y_ > pointB_.y_ && pointA_.y_ > pointC_.y_ ? pointA_.y_ :
                 pointB_.y_ > pointC_.y_ ? pointB_.y_ : pointC_.y_};
  const float z {pointA_.z_ > pointB_.z_ && pointA_.z_ > pointC_.z_ ? pointA_.z_ :
                 pointB_.z_ > pointC_.z_ ? pointB_.z_ : pointC_.z_};
  return Point3D (x, y, z);
}

AABB Triangle::getAABB() const noexcept {
  return AABB {getPositionMin (), getPositionMax ()};
}

bool Triangle::intersect (const AABB box) const noexcept {
  std::function<bool (Point3D orig, Vector3D vec)> intersectRayAABB {
    [&](Point3D orig, Vector3D vec)->bool {
      float tmin {(box.pointMin_.x_ - orig.x_) / vec.x_};
      float tmax {(box.pointMax_.x_ - orig.x_) / vec.x_};

      if (tmin > tmax) {
        std::swap(tmin, tmax);
      }
      float tymin {(box.pointMin_.y_ - orig.y_) / vec.y_};
      float tymax {(box.pointMax_.y_ - orig.y_) / vec.y_};

      if (tymin > tymax) {
        std::swap(tymin, tymax);
      }

      if ((tmin > tymax) || (tymin > tmax)) {
        return false;
      }

      if (tymin > tmin) {
        tmin = tymin;
      }

      if (tymax < tmax) {
        tmax = tymax;
      }
      float tzmin {(box.pointMin_.z_ - orig.z_) / vec.z_};
      float tzmax {(box.pointMax_.z_ - orig.z_) / vec.z_};

      if (tzmin > tzmax) {
        std::swap(tzmin, tzmax);
      }

      if ((tmin > tzmax) || (tzmin > tmax)) {
        return false;
      }

      if (tzmin > tmin) {
        tmin = tzmin;
      }

      if (tzmax < tmax) {
        tmax = tzmax;
      }
      return true;
    }};

    return intersectRayAABB(this->pointA_, this->AB_) ||
           intersectRayAABB(this->pointA_, this->AC_) ||
           intersectRayAABB(this->pointB_, this->BC_);
}
