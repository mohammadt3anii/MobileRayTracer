//
// Created by Tiago on 16-10-2016.
//

#include "PointLight.h"

using namespace Components;

PointLight::PointLight(const Material &radiance, const Point3D &position) noexcept :
        Light(radiance),
        position_(position) {
}

const Point3D PointLight::getPosition(void) noexcept {
    return this->position_;
}

void PointLight::resetSampling(void) noexcept {
}

bool PointLight::intersect(Intersection &, const Ray &, const Material &) const noexcept {
    return false;
}
