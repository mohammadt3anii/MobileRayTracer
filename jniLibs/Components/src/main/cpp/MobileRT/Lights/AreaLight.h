//
// Created by puscas on 03-03-2017.
//

#ifndef MOBILERAYTRACER_AREALIGHT_H
#define MOBILERAYTRACER_AREALIGHT_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Lights/Light.h"
#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class AreaLight : public Light {
    private:
        const float sizeX_;
        const float sizeZ_;
        Sampler& sampler_;

    public:
        explicit AreaLight(const RGB &radiance, const Point3D &position,
            const float sizeX, const float sizeZ, Sampler &sampler);
        virtual Point3D getPosition (void) override;
    };
}

#endif //MOBILERAYTRACER_AREALIGHT_H
