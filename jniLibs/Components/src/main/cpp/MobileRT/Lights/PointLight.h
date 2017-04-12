//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINTLIGHT_H
#define MOBILERAYTRACER_POINTLIGHT_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Lights/Light.h"
//#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class PointLight : public Light {
    private:
        Point3D position_;

    public:
        explicit PointLight(const RGB &radiance, const Point3D &position);

        virtual const Point3D getPosition(void) override;

        virtual void resetSampling(void) override;
    };
}

#endif //MOBILERAYTRACER_POINTLIGHT_H
