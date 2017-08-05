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

AreaLight::AreaLight(const Material &radiance,
										 std::unique_ptr<Sampler> &&samplerPointLight,
                     const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) noexcept :
        Light(radiance),
        triangle_(pointA, pointB, pointC),
        samplerPointLight_(std::move(samplerPointLight)) {
}

Point3D AreaLight::getPosition() noexcept {

  float R(samplerPointLight_->getSample(0));
  float S(samplerPointLight_->getSample(0));
    if (R + S >= 1.0f) {
        R = 1.0f - R;
        S = 1.0f - S;
    }
    return Point3D(triangle_.pointA_.x_ + R * triangle_.AB_.x_ + S * triangle_.AC_.x_,
                   triangle_.pointA_.y_ + R * triangle_.AB_.y_ + S * triangle_.AC_.y_,
                   triangle_.pointA_.z_ + R * triangle_.AB_.z_ + S * triangle_.AC_.z_);
}

void AreaLight::resetSampling() noexcept {
    samplerPointLight_->resetSampling();
}

bool AreaLight::intersect(Intersection *intersection, const Ray &ray) const noexcept {
    if (triangle_.intersect(intersection, ray)) {
			intersection->material_ = &radiance_;
			return true;
		}
		return false;
}
