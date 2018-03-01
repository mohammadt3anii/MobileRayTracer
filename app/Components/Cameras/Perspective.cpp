//
// Created by Tiago on 16-10-2016.
//

#include "Components/Cameras/Perspective.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using ::Components::Perspective;
using ::MobileRT::Ray;

Perspective::Perspective(const ::glm::vec3 position, const ::glm::vec3 lookAt, const ::glm::vec3 up,
                         const float hFov, const float vFov) noexcept :
        Camera(position, lookAt, up),
        // convert to radians
        hFov_{(hFov * ::glm::pi<float>()) / 180.0f},
        vFov_{(vFov * ::glm::pi<float>()) / 180.0f} {
}

/* u = x / width */
/* v = y / height */
/* deviationU = [-0.5f / width, 0.5f / width] */
/* deviationV = [-0.5f / height, 0.5f / height] */
Ray Perspective::generateRay(const float u, const float v,
                             const float deviationU, const float deviationV) const noexcept {
    const ::glm::vec3 dest {this->position_ +
                          this->direction_ +
                          (this->right_ * (fastArcTan(this->hFov_ * (u - 0.5f)) + deviationU)) +
                          (this->up_ * (fastArcTan(this->vFov_ * (0.5f - v)) + deviationV))};
    return Ray {::glm::normalize(dest - position_),
                    this->position_, 1};
}

//http://nghiaho.com/?p=997
float Perspective::fastArcTan(const float value) const noexcept {
    const float absValue{::std::abs(value)};
    return ::glm::quarter_pi<float>() * value -
           (value * (absValue - 1.0f)) * (0.2447f + (0.0663f * absValue));
}
