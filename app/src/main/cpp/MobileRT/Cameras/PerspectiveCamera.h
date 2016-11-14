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
            const float hFov_;
            const float vFov_;
            const Point3D position_;

            float FastArcTan(const float x) const;

        public:
            PerspectiveCamera(const Point3D& position, const float hFov, const float vFov);
            void getRay(const float u, const float v, Ray& ray) const;
    };
}

#endif //MOBILERAYTRACER_PERSPECTIVECAMERA_H