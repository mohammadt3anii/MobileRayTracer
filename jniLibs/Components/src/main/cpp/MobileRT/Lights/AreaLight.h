//
// Created by puscas on 03-03-2017.
//

#ifndef MOBILERAYTRACER_AREALIGHT_H
#define MOBILERAYTRACER_AREALIGHT_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Lights/Light.h"

namespace MobileRT {
    class AreaLight : public Light {
    private:

    public:
        explicit AreaLight(const RGB &radiance, const Point3D &position);
    };
}

#endif //MOBILERAYTRACER_AREALIGHT_H
