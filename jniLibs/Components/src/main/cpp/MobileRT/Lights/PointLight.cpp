//
// Created by Tiago on 16-10-2016.
//

#include "PointLight.h"

using namespace MobileRT;

PointLight::PointLight(const Material &radiance, const Point3D &position) :
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

bool PointLight::intersect(Intersection &, const Ray &, const Material&) const {
        return false;
}
