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

/*
package com.example.lps.mobilert;

public class RTCamera {

    // viewing foveal angle
private double hfov, vfov;
private myPoint orig;

    RTCamera (myPoint ppos, float phfov, float pvfov) {
        hfov = (double)phfov; // in degrees
        vfov = (double)pvfov; // in degrees
        // convert to radians
        hfov *= Math.PI  / 180.;
        vfov *= Math.PI  / 180.;
        orig = ppos;
    }

public Ray getRay (float u, float v) {
        double u_alpha, v_alpha;
        myVect dir;

        u_alpha = hfov * (u-0.5);
        v_alpha = -vfov * (v-0.5);

        dir = new myVect((float)Math.atan(u_alpha), (float)Math.atan(v_alpha), 1.f);

        return new Ray (orig, dir);  // note that depth will be 0 : primary ray

    }
}
*/