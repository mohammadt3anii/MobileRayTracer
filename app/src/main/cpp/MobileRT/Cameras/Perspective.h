//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PERSPECTIVE_H
#define MOBILERAYTRACER_PERSPECTIVE_H

#include "../Point3D.h"
#include "../Ray.h"

namespace MobileRT {
    class Perspective {
    private:
        const Point3D position_;//looking at Z+

    public:
        const float hFov_;
        const float vFov_;

        Perspective(const Point3D &position, const float hFov, const float vFov);

        ~Perspective();

        void getRay(Ray &ray, const float u_alpha, const float v_alpha) const;
    };
}

#endif //MOBILERAYTRACER_PERSPECTIVE_H