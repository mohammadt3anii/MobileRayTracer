//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_CAMERA_H
#define MOBILERAYTRACER_CAMERA_H

#include "../Ray.h"

namespace MobileRT {
    class Camera {

    public:
        Camera() = default;
        virtual ~Camera() = default;
        virtual void getRay(Ray &ray, const float u_alpha, const float v_alpha) const = 0;
    };
}

#endif //MOBILERAYTRACER_CAMERA_H