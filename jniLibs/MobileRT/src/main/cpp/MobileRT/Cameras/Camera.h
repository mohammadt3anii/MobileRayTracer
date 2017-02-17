//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_CAMERA_H
#define MOBILERAYTRACER_CAMERA_H

#include "../Point3D.h"
#include "../Ray.h"

namespace MobileRT {
    class Camera {
    protected:
        const Point3D position_;//looking at Z+

    public:
        const float hFov_;
        const float vFov_;

    public:
        Camera(const Point3D &position, const float hFov, const float vFov);
        virtual void getRay(Ray &ray, const float u_alpha, const float v_alpha) const = 0;

        virtual ~Camera();
    };
}

#endif //MOBILERAYTRACER_CAMERA_H
