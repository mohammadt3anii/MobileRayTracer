//
// Created by Tiago on 16-10-2016.
//

#include "Perspective.hpp"

using Components::Perspective;
using MobileRT::Point3D;
using MobileRT::Vector3D;
using MobileRT::Ray;

Perspective::Perspective(const Point3D &position, const Point3D &lookAt, const Vector3D &up,
                         const float hFov, const float vFov) noexcept :
        Camera(position, lookAt, up),
        // convert to radians
        hFov_((hFov * PI) / 180.0f),
        vFov_((vFov * PI) / 180.0f) {
}

Ray Perspective::generateRay(const float u, const float v,
                                   const float deviationU, const float deviationV) const noexcept {
    const float u_alpha(fastArcTan(this->hFov_ * (u - 0.5f)) + deviationU);
    const float v_alpha(fastArcTan(this->vFov_ * (0.5f - v)) + deviationV);

    const Point3D imagePoint(this->position_ + this->direction_ +
                             (this->right_ * u_alpha) + (this->up_ * v_alpha));

    const Vector3D rayDirection(imagePoint, this->position_, true);

    return Ray(rayDirection, this->position_, 1u);
}

//http://nghiaho.com/?p=997
float Perspective::fastArcTan(const float value) const noexcept {
    const float absValue(std::fabs(value));
    return PI_4 * value - (value * (absValue - 1.0f)) * (0.2447f + (0.0663f * absValue));
}
