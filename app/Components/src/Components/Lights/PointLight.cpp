//
// Created by Tiago on 16-10-2016.
//

#include "PointLight.hpp"

using ::Components::PointLight;
using ::MobileRT::Material;
using ::MobileRT::Point3D;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;

PointLight::PointLight(Material radiance, const Point3D position) noexcept :
        Light(::std::move(radiance)),
        position_{position} {
}

Point3D PointLight::getPosition() noexcept {
    return this->position_;
}

void PointLight::resetSampling() noexcept {
}

bool
PointLight::intersect(Intersection *const /*intersection*/, const Ray /*ray*/) const noexcept {
    return false;
}
