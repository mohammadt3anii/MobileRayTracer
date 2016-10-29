//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RTCAMERA_H
#define MOBILERAYTRACER_RTCAMERA_H

#include "Point.h"
#include "Ray.h"

namespace MobileRT {
    class RTCamera {
    private:
        double hfov, vfov;
        Point* orig;

    public:
        RTCamera (Point* ppos, float phfov, float pvfov);
        Ray* getRay (float u, float v);
    };
}

#endif //MOBILERAYTRACER_RTCAMERA_H