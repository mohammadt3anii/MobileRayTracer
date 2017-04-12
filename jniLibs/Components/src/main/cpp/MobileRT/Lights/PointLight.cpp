//
// Created by Tiago on 16-10-2016.
//

#include "PointLight.h"

using namespace MobileRT;

PointLight::PointLight(const RGB &radiance, const Point3D &position) :
        Light(radiance),
        position_(position)
{
}

const Point3D PointLight::getPosition(void)
{
        return this->position_;
}

void PointLight::resetSampling(void) {
}
