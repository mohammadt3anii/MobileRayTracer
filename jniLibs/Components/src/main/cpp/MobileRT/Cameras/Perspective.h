//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PERSPECTIVE_H
#define MOBILERAYTRACER_PERSPECTIVE_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Cameras/Camera.h"
#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Utils.h"

namespace MobileRT {
    class Perspective : public Camera {

    public:
        explicit Perspective(const Point3D &position, const float hFov, const float vFov);

        ~Perspective(void);

        virtual void getRay(Ray &ray, const float u_alpha, const float v_alpha) const override;
    };
}

#endif //MOBILERAYTRACER_PERSPECTIVE_H
