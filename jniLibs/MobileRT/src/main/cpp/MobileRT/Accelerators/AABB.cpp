//
// Created by puscas on 06-03-2017.
//

#include "AABB.hpp"

using MobileRT::AABB;
using MobileRT::Ray;

AABB::AABB (const Point3D pointMin, const Point3D pointMax) noexcept :
  pointMin_ {pointMin}, pointMax_ {pointMax} {
}

float AABB::intersect (Ray ray) const noexcept {
  //Ray ray2 {ray};
  Vector3D dir_inv {1 / ray.direction_};
  dir_inv.normalize ();
  float t1 {(pointMin_.x_ - ray.origin_.x_) * dir_inv.x_};
  float t2 {(pointMax_.x_ - ray.origin_.x_) * dir_inv.x_};
  float tmin {std::min (t1, t2)};
  float tmax {std::max (t1, t2)};
  t1 = (pointMin_.y_ - ray.origin_.y_) * dir_inv.y_;
  t2 = (pointMax_.y_ - ray.origin_.y_) * dir_inv.y_;
  tmin = std::max (tmin, std::min (std::min (t1, t2), tmax));
  tmax = std::min (tmax, std::max (std::max (t1, t2), tmin));
  t1 = (pointMin_.z_ - ray.origin_.z_) * dir_inv.z_;
  t2 = (pointMax_.z_ - ray.origin_.z_) * dir_inv.z_;
  tmin = std::max (tmin, std::min (std::min (t1, t2), tmax));
  tmax = std::min (tmax, std::max (std::max (t1, t2), tmin));
  const bool res {tmax > std::max (tmin, 0.0f)};
  return res;
}
