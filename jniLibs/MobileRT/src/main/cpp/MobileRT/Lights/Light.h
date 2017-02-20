//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "../RGB.h"
#include "../Point3D.h"

namespace MobileRT {
    class Light {
    public:
        RGB radiance_;
        Point3D position_;

    public:
        explicit Light(const RGB &radiance, const Point3D &position);
    };
}

#endif //MOBILERAYTRACER_LIGHT_H
