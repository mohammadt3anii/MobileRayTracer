//
// Created by puscas on 03-03-2017.
//

#ifndef MOBILERAYTRACER_AREALIGHT_H
#define MOBILERAYTRACER_AREALIGHT_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Lights/Light.h"
#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Shapes/Triangle.h"
#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"
#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Utils.h"

namespace MobileRT {
    class AreaLight : public Light, public Triangle {
    private:
        Sampler& sampler_;

    public:
        explicit AreaLight(const RGB &radiance, const Point3D &position, Sampler &sampler,
                           const Point3D &pointA, const Point3D &pointB, const Point3D &pointC);

        virtual Point3D getPosition(const unsigned int i, const unsigned int j) override;

        virtual void resetSampling(void) override;
    };
}

#endif //MOBILERAYTRACER_AREALIGHT_H
