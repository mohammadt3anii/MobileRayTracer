//
// Created by Tiago on 16-10-2016.
//

#include "Ray.hpp"
#include <atomic>

using MobileRT::Ray;
static unsigned counter {0};
namespace {
  uint32_t getID () {
    static std::atomic<uint32_t> id {0};
    const uint32_t current {id.fetch_add (1, std::memory_order_relaxed)};
    return current;
  }
}//namespace

Ray::Ray(Vector3D dir, Point3D origin,
         const unsigned depth) noexcept :
  origin_ {origin},
  direction_ {dir},
  depth_ {depth},
  id_ {getID ()} {
    counter++;
}

unsigned Ray::getInstances () noexcept {
  const unsigned res {counter};
  counter = 0;
  return res;
}
