//
// Created by puscas on 16-10-2016.
//

#include "RTCamera.h"
#include <math.h>
#include <memory>

using namespace MobileRT;

RTCamera::RTCamera(const Point &position, const float &hFov, const float &vFov) :
        // convert to radians
        hFov_(hFov * M_PI / 180.),
        vFov_(vFov * M_PI / 180.),
        position_(std::move(position)) {
}

float RTCamera::FastArcTan(const float x) const {
    return M_PI_4*x - x*(fabs(x) - 1)*(0.2447 + 0.0663*fabs(x));
}

Ray RTCamera::getRay(const float u, const float v) {
    float u_alpha = hFov_ * (u - 0.5);
    float v_alpha = -vFov_ * (v - 0.5);
    Vect dir = Vect(static_cast<float>(FastArcTan(u_alpha)), static_cast<float>(FastArcTan(v_alpha)), 1.f);
    // note that depth will be 0 : primary ray
    return Ray(position_, dir);
}