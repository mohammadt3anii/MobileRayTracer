//
// Created by Tiago on 23-Jan-17.
//

#include "Camera.hpp"

using MobileRT::Camera;
namespace {
  static std::array<float, NumberOfBlocks> VALUES;

  static bool FillThings () {
    for (uint32_t i {0}; i < NumberOfBlocks; ++i) {
      VALUES[i] = haltonSequence (i, 2);
    }
    std::random_shuffle (VALUES.begin (), VALUES.end ());
    return true;
  }
}

float Camera::getBlock (unsigned int sample) noexcept {
  const unsigned current {this->block_.fetch_add (1, std::memory_order_relaxed)};
  if (current >= (NumberOfBlocks * (sample + 1))) {
    this->block_.fetch_sub (1, std::memory_order_relaxed);
    return 1.0f;
  }
  return VALUES[current];
}

//Left hand rule
Camera::Camera (Point3D position, Point3D lookAt, Vector3D up) noexcept :
  position_ {position},
  direction_ {Vector3D (lookAt, position, true)},
  right_ {up . crossProduct (direction_)},
  up_ {direction_ . crossProduct (right_)} {
  static bool unused {FillThings ()};
}

Camera::~Camera () noexcept {
	LOG("CAMERA DELETED");
}
