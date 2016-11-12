//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RTCAMERA_H
#define MOBILERAYTRACER_RTCAMERA_H

#include "Point.h"
#include "Ray.h"

namespace MobileRT
{
    class RTCamera
    {
        private:
            const float hFov_;
            const float vFov_;
            Point position_;

        public:
            RTCamera(const Point& position, const float hFov, const float vFov);
            float FastArcTan(const float x) const;
            void getRay(const float u, const float v, Ray& ray) const;
    };
}

#endif //MOBILERAYTRACER_RTCAMERA_H