//
// Created by puscas on 06-03-2017.
//

#include "AABB.hpp"

using MobileRT::AABB;

AABB::AABB (const Point3D pointMin, const Point3D pointMax) noexcept :
  pointMin_ {pointMin}, pointMax_ {pointMax} {
}
