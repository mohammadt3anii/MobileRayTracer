//
// Created by Tiago on 16-10-2016.
//

#include "Perspective.hpp"

using ::Components::Perspective;
using ::MobileRT::Point3D;
using ::MobileRT::Vector3D;
using ::MobileRT::Ray;

Perspective::Perspective(const Point3D position, const Point3D lookAt, const Vector3D up,
                         const float hFov, const float vFov) noexcept :
        Camera{position, lookAt, up},
        // convert to radians
        hFov_{(hFov * ::MobileRT::Pi) / 180.0f},
        vFov_{(vFov * ::MobileRT::Pi) / 180.0f} {
}

/* u = x / width */
/* v = y / height */
/* deviationU = [-0.5f / width, 0.5f / width] */
/* deviationV = [-0.5f / height, 0.5f / height] */
Ray Perspective::generateRay(const float u, const float v,
                             const float deviationU, const float deviationV) const noexcept {
    return Ray {Vector3D {this->position_ +
                          this->direction_ +
                          (this->right_ * (fastArcTan(this->hFov_ * (u - 0.5f)) + deviationU)) +
                          (this->up_ * (fastArcTan(this->vFov_ * (0.5f - v)) + deviationV)),
                          this->position_, true},
                this->position_, 1};
}

//http://nghiaho.com/?p=997
float Perspective::fastArcTan(const float value) const noexcept {
    const float absValue{value < 0.0f ? -value : value};
    return ::MobileRT::Pi_4 * value -
           (value * (absValue - 1.0f)) * (0.2447f + (0.0663f * absValue));
}
