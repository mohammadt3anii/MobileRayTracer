//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PERSPECTIVECAMERA_H
#define MOBILERAYTRACER_PERSPECTIVECAMERA_H

#include "../Point3D.h"
#include "../Ray.h"

namespace MobileRT
{
    class PerspectiveCamera
    {
        private:
            const Point3D position_;//a olhar para o eixo +Z

        public:
            const float hFov_;
            const float vFov_;

            PerspectiveCamera(const Point3D& position, const float hFov, const float vFov);
            void getRay(Ray& ray, const float u_alpha, const float v_alpha) const;
            Ray getRay(const float u, const float v) const;
    };
}

#endif //MOBILERAYTRACER_PERSPECTIVECAMERA_H