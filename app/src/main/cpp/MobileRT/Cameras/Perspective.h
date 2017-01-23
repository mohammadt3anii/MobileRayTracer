//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PERSPECTIVE_H
#define MOBILERAYTRACER_PERSPECTIVE_H

#include "Camera.h"
#include "../Utils.h"

namespace MobileRT {
    class Perspective : public Camera {
    private:

    public:
        Perspective(const Point3D &position, const float hFov, const float vFov);

        ~Perspective();

        void getRay(Ray &ray, const float u_alpha, const float v_alpha) const;
    };
}

#endif //MOBILERAYTRACER_PERSPECTIVE_H