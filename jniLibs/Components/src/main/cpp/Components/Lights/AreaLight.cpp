//
// Created by puscas on 03-03-2017.
//

#include "AreaLight.hpp"

using Components::AreaLight;
using MobileRT::Material;
using MobileRT::Sampler;
using MobileRT::Point3D;
using MobileRT::Ray;
using MobileRT::Intersection;
using MobileRT::Triangle;

AreaLight::AreaLight(const Material &radiance, Sampler &samplerPointLight,
                     const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) noexcept :
        Light(radiance),
        Triangle(pointA, pointB, pointC),
        samplerPointLight_(samplerPointLight) {
}

Point3D AreaLight::getPosition() noexcept {
    float R(samplerPointLight_.getSample(0u));
    float S(samplerPointLight_.getSample(0u));
    if (R + S >= 1.0f) {
        R = 1.0f - R;
        S = 1.0f - S;
    }
    return Point3D(pointA_.x_ + R * AB_.x_ + S * AC_.x_,
                   pointA_.y_ + R * AB_.y_ + S * AC_.y_,
                   pointA_.z_ + R * AB_.z_ + S * AC_.z_);
}

void AreaLight::resetSampling() noexcept {
    samplerPointLight_.resetSampling();
}

bool AreaLight::intersect(Intersection &intersection, const Ray &ray) const noexcept {
    return Triangle::intersect(intersection, ray, this->radiance_);
}
