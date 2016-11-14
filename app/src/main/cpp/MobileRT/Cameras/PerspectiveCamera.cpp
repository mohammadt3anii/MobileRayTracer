//
// Created by Tiago on 16-10-2016.
//

#include "PerspectiveCamera.h"
#include "../Constants.h"
//#include <cmath>

using namespace MobileRT;

PerspectiveCamera::PerspectiveCamera(const Point3D& position, const float hFov, const float vFov) :
        position_(position),
        // convert to radians
        hFov_(hFov * M_PI / 180.0f),
        vFov_(vFov * M_PI / 180.0f)
{
}

void PerspectiveCamera::getRay(Ray& ray, const float u_alpha, const float v_alpha) const
{
    // note that depth will be 0 : primary ray
    //void recycle (const float x, const float y, const float z, const Point3D& origin);
    ray.recycle (u_alpha, v_alpha, 1.0f, this->position_);
}