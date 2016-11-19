//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINTLIGHT_H
#define MOBILERAYTRACER_POINTLIGHT_H

#include "../Color_Models/RGB.h"
#include "../Point3D.h"

namespace MobileRT {
    class PointLight {
    private:


    public:
        const RGB radiance_;
        const Point3D position_;

        PointLight(const RGB &radiance, const Point3D &position);
    };
}

#endif //MOBILERAYTRACER_POINTLIGHT_H