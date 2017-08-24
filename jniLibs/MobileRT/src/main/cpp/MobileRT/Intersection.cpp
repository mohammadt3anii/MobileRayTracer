//
// Created by Tiago on 16-10-2016.
//

#include "Intersection.hpp"

using MobileRT::Intersection;
static unsigned counter {0};

Intersection::Intersection (const void *primitive) noexcept :
  primitive_ {primitive} {
}

void Intersection::reset (const Point3D orig,
                          const Vector3D dir,
                          const float dist,
                          const Vector3D normal, const void *const primitive) noexcept {
    this->point_.x_ = orig.x_ + dir.x_ * dist;
    this->point_.y_ = orig.y_ + dir.y_ * dist;
    this->point_.z_ = orig.z_ + dir.z_ * dist;
    this->normal_ = normal;
    this->symNormal_.x_ = -normal.x_;
    this->symNormal_.y_ = -normal.y_;
    this->symNormal_.z_ = -normal.z_;
    this->length_ = dist;
    this->primitive_ = primitive;
}

void Intersection::reset (const Point3D orig,
                          const Vector3D dir,
                          const float dist,
                          const Point3D center) noexcept {
    this->point_.x_ = orig.x_ + dir.x_ * dist;
    this->point_.y_ = orig.y_ + dir.y_ * dist;
    this->point_.z_ = orig.z_ + dir.z_ * dist;
    this->normal_ = Vector3D(this->point_, center, true);
    this->symNormal_.x_ = -this->normal_.x_;
    this->symNormal_.y_ = -this->normal_.y_;
    this->symNormal_.z_ = -this->normal_.z_;
    this->length_ = dist;
}

unsigned Intersection::getInstances () noexcept {
    const unsigned res {counter};
    counter = 0;
    return res;
}
