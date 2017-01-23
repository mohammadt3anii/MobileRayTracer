//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "../Tone_Mappers/RGB.h"
#include "../Point3D.h"

namespace MobileRT {
    class Light {
    private:

    public:
        const RGB radiance_;
        const Point3D position_;

        Light(const RGB &radiance, const Point3D &position);
    };
}

#endif //MOBILERAYTRACER_LIGHT_H