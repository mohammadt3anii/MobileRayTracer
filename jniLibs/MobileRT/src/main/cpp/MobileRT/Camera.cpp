//
// Created by Tiago on 23-Jan-17.
//

#include "Camera.hpp"

using MobileRT::Camera;

//Left hand rule
Camera::Camera(const Point3D &position, const Point3D &lookAt, const Vector3D &up) noexcept :
  position_ {position},
  direction_ {Vector3D (lookAt, position, true)},
  right_ {up . crossProduct (direction_)},
  up_ {direction_ . crossProduct (right_)} {
}

Camera::~Camera () noexcept {
	LOG("CAMERA DELETED");
}
