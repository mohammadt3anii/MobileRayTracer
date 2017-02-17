//
// Created by Tiago on 23-Jan-17.
//

#include "Camera.h"

using namespace MobileRT;

Camera::Camera(const Point3D &position, const float hFov, const float vFov) :
        position_(position),
        // convert to radians
        hFov_((hFov * PI) / 180.0f),
        vFov_((vFov * PI) / 180.0f) {
}

Camera::~Camera() {
}
