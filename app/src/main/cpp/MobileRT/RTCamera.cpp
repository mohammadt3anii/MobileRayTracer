//
// Created by puscas on 16-10-2016.
//

#include "RTCamera.h"
#include <math.h>

using namespace MobileRT;

RTCamera::RTCamera (Point* ppos, float phfov, float pvfov) {
    hfov = (double)phfov; // in degrees
    vfov = (double)pvfov; // in degrees
    // convert to radians
    hfov *= M_PI  / 180.;
    vfov *= M_PI  / 180.;
    orig = ppos;
}

Ray* RTCamera::getRay (float u, float v) {
    double u_alpha, v_alpha;
    Vect* dir;

    u_alpha = hfov * (u-0.5);
    v_alpha = -vfov * (v-0.5);

    dir = new Vect((float)atan(u_alpha), (float)atan(v_alpha), 1.f);

    return new Ray (orig, dir);  // note that depth will be 0 : primary ray
}