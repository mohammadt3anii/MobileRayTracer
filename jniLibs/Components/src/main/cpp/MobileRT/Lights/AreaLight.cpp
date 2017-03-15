//
// Created by puscas on 03-03-2017.
//

#include "AreaLight.h"

using namespace MobileRT;

AreaLight::AreaLight(const RGB &radiance, const Point3D &position,
        const float sizeX, const float sizeZ, Sampler &sampler) :
        Light(radiance, position),
        sizeX_(sizeX),
        sizeZ_(sizeZ),
        sampler_(sampler)
{
}

Point3D AreaLight::getPosition (void)
{
        const float deviationX (sampler_.getSample(0) * 2 - 1);
        const float deviationZ (sampler_.getSample(0) * 2 - 1);
        Point3D position (position_.x_+sizeX_*deviationX, position_.y_, position_.z_+sizeZ_*deviationZ);
        return position;
}
