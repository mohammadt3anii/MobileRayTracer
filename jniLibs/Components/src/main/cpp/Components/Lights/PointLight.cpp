//
// Created by Tiago on 16-10-2016.
//

#include "PointLight.hpp"

using Components::PointLight;

PointLight::PointLight(const Material &radiance, const Point3D &position) noexcept :
        Light(radiance),
        position_(position) {
}

const Point3D PointLight::getPosition() noexcept {
    return this->position_;
}

void PointLight::resetSampling() noexcept {
}

bool PointLight::intersectL(Intersection &/*intersection*/, const Ray &/*ray*/) const noexcept {
    return false;
}
