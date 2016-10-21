//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RTCAMERA_H
#define MOBILERAYTRACER_RTCAMERA_H

#include "myPoint.h"
#include "Ray.h"

namespace MobileRT {
    class RTCamera {
    private:
        double hfov, vfov;
        myPoint* orig;

    public:
        RTCamera (myPoint* ppos, float phfov, float pvfov);
        Ray* getRay (float u, float v);
    };
}

#endif //MOBILERAYTRACER_RTCAMERA_H