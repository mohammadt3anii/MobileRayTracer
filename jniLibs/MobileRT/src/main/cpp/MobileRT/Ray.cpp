//
// Created by Tiago on 16-10-2016.
//

#include "Ray.hpp"

using MobileRT::Ray;
static unsigned counter {0};

Ray::Ray(Vector3D dir, Point3D origin,
         const unsigned depth) noexcept :
  origin_ {origin},
  direction_ {dir},
  depth_ {depth} {
    counter++;
}

unsigned Ray::getInstances () noexcept {
  const unsigned res {counter};
  counter = 0;
  return res;
}
