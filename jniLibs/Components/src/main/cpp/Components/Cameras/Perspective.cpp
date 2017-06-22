//
// Created by Tiago on 16-10-2016.
//

#include "Perspective.h"

using namespace Components;

Perspective::Perspective(const Point3D &position, const Point3D &lookAt, const Vector3D &up,
                         const float hFov, const float vFov) :
        Camera(position, lookAt, up),
        // convert to radians
        hFov_((hFov * PI) / 180.0f),
        vFov_((vFov * PI) / 180.0f) {
}

Perspective::~Perspective(void) {
}

Ray Perspective::generateRay(const float u, const float v,
                             const float deviationU, const float deviationV) const {
    const float u_alpha(fastArcTan(this->hFov_ * (u - 0.5f)) + deviationU);
    const float v_alpha(fastArcTan(this->vFov_ * (0.5f - v)) + deviationV);

    const Point3D imagePoint(this->position_ + this->direction_ +
                             (this->right_ * u_alpha) + (this->up_ * v_alpha));

    const Vector3D rayDirection(imagePoint - this->position_);

    return Ray(rayDirection.returnNormalized(), this->position_, 1);
}
