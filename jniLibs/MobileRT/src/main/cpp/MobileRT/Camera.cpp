//
// Created by Tiago on 23-Jan-17.
//

#include "Camera.h"

using namespace MobileRT;

//Left hand rule
Camera::Camera(const Point3D &position, const Point3D &lookAt, const Vector3D &up) :
        position_(position),
        direction_((lookAt - position).returnNormalized()),
        right_((up.crossProduct(direction_)).returnNormalized()),
        up_((direction_.crossProduct(right_)).returnNormalized()) {
}

Camera::~Camera(void) {
}
