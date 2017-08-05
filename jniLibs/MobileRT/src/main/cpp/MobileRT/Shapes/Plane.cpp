//
// Created by Tiago on 16-10-2016.
//

#include "Plane.hpp"

using MobileRT::AABB;
using MobileRT::Plane;
using MobileRT::Point3D;

Plane::Plane(const Point3D &point, const Vector3D &normal) noexcept :
        normal_(normal),
        point_(point)
{
}

bool Plane::intersect(Intersection *intersection, const Ray &ray) const noexcept {
    // is ray parallel or contained in the Plane ??
    // planes have two sides!!!
    const float normalized_projection(this->normal_.dotProduct(ray.direction_));
    if (std::fabs(normalized_projection) < EPSILON) {
        return false;
	}

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const float distanceToIntersection(
            this->normal_.dotProduct(this->point_, ray.origin_) / normalized_projection);

    // is it in front of the eye?
    // is it farther than the ray length ??
    if (distanceToIntersection < EPSILON || distanceToIntersection > intersection->length_) {
        return false;
	}

    // if so, then we have an intersection
    intersection->reset(ray.origin_, ray.direction_, distanceToIntersection,
                       this->normal_);

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

AABB Plane::getAABB() const noexcept {
	return AABB(getPositionMin(), getPositionMax());
}

bool Plane::intersect(const AABB &box) const noexcept {

  Point3D positiveVertex(box.pointMin_);
  Point3D negativeVertex(box.pointMax_);
  if (this->normal_.x_ >= 0.0f) {
    positiveVertex.x_ = box.pointMax_.x_;
  }
  if (this->normal_.y_ >= 0.0f) {
    positiveVertex.y_ = box.pointMax_.y_;
  }
  if (this->normal_.z_ >= 0.0f) {
    positiveVertex.z_ = box.pointMax_.z_;
  }

  if (this->normal_.x_ >= 0.0f) {
    negativeVertex.x_ = box.pointMin_.x_;
  }
  if (this->normal_.y_ >= 0.0f) {
    negativeVertex.y_ = box.pointMin_.y_;
  }
  if (this->normal_.z_ >= 0.0f) {
    negativeVertex.z_ = box.pointMin_.z_;
  }

  // is the positive vertex outside?
  /*if (this->distance(positiveVertex(this->normal_)) < 0) {
    return false;
  }*/
  // is the negative vertex outside?
  /*else if (this->distance(negativeVertex(this->normal_)) < 0) {
    return true;
  }*/

  Intersection intersectionPositive;
  Intersection intersectionNegative;
  Ray rayPositive(this->normal_, positiveVertex, 1);
  Ray rayNegative(this->normal_, negativeVertex, 1);
  if (intersect(&intersectionPositive, rayPositive)) {
    if (intersectionPositive.length_ < 0) {
      return false;
    }
  } else if (intersect(&intersectionNegative, rayNegative)) {
    if (intersectionNegative.length_ < 0) {
      return true;
    }
  }

  return false;
}
