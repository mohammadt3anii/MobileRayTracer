//
// Created by Tiago on 16-10-2016.
//

#include "Ray.hpp"

using MobileRT::Ray;

static unsigned int counter(0);

Ray::Ray(Vector3D dir, Point3D origin,
         const unsigned int depth) noexcept :
  origin_ {origin},
  direction_ {dir},
  symDirection_ {- direction_},
  depth_ {depth} {
    counter++;
}

unsigned int Ray::getInstances() noexcept {

  const unsigned int res {counter};
    counter = 0;
    return res;
}
