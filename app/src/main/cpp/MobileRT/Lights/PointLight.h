//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINTLIGHT_H
#define MOBILERAYTRACER_POINTLIGHT_H

#include "Light.h"

namespace MobileRT {
    class PointLight : public Light {
    private:


    public:
        PointLight(const RGB &radiance, const Point3D &position);
    };
}

#endif //MOBILERAYTRACER_POINTLIGHT_H