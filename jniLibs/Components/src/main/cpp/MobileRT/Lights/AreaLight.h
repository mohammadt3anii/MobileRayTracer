//
// Created by puscas on 03-03-2017.
//

#ifndef MOBILERAYTRACER_AREALIGHT_H
#define MOBILERAYTRACER_AREALIGHT_H

#include "MobileRT/Lights/Light.h"
#include "MobileRT/Shapes/Triangle.h"
#include "MobileRT/Samplers/Sampler.h"
#include "MobileRT/Utils.h"

namespace MobileRT {
    class AreaLight : public Light, public Triangle {
    private:
        Sampler& samplerPointLight_;

    public:
        explicit AreaLight(const RGB &radiance, Sampler &samplerPointLight,
                           const Point3D &pointA, const Point3D &pointB, const Point3D &pointC);

        virtual const Point3D getPosition(void) override;

        virtual void resetSampling(void) override;
    };
}

#endif //MOBILERAYTRACER_AREALIGHT_H
