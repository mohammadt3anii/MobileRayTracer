//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_CAMERA_H
#define MOBILERAYTRACER_CAMERA_H

#include "../Ray.h"

namespace MobileRT {
    class Camera {
        virtual void getRay(Ray &ray, const float u_alpha, const float v_alpha) const = 0;
    };
}

#endif //MOBILERAYTRACER_CAMERA_H