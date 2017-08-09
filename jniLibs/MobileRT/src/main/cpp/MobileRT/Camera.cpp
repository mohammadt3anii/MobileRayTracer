//
// Created by Tiago on 23-Jan-17.
//

#include "Camera.hpp"

using MobileRT::Camera;

//Left hand rule
Camera::Camera (Point3D position, Point3D lookAt, Vector3D up) noexcept :
  position_ {position},
  direction_ {Vector3D (lookAt, position, true)},
  right_ {up . crossProduct (direction_)},
  up_ {direction_ . crossProduct (right_)} {
}

Camera::~Camera () noexcept {
	LOG("CAMERA DELETED");
}
