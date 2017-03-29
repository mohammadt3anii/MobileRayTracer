//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINTLIGHT_H
#define MOBILERAYTRACER_POINTLIGHT_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Lights/Light.h"

namespace MobileRT {
    class PointLight : public Light {
    public:
        explicit PointLight(const RGB &radiance, const Point3D &position);

        virtual Point3D getPosition(const unsigned int i, const unsigned int j) override;

        virtual void resetSampling(void) override;
    };
}

#endif //MOBILERAYTRACER_POINTLIGHT_H
