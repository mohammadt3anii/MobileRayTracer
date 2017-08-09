//
// Created by puscas on 06-03-2017.
//

#include "AABB.hpp"

using MobileRT::AABB;

AABB::AABB (Point3D pointMin, Point3D pointMax) noexcept :
  pointMin_ {pointMin}, pointMax_ {pointMax} {
}
