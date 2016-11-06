//
// Created by puscas on 16-10-2016.
//

#include "RTCamera.h"
#include <math.h>
#include <memory>

using namespace MobileRT;

RTCamera::RTCamera(const Point &position, const float &hFov, const float &vFov) :
        position_(std::move(position)),
        // convert to radians
        hFov_(hFov * M_PI / 180.),
        vFov_(vFov * M_PI / 180.) {
}

Ray RTCamera::getRay(float u, float v) {
    float u_alpha = hFov_ * (u - 0.5);
    float v_alpha = -vFov_ * (v - 0.5);
    Vect dir = Vect(static_cast<float>(atan(u_alpha)), static_cast<float>(atan(v_alpha)), 1.f);
    // note that depth will be 0 : primary ray
    return Ray(position_, dir);
}