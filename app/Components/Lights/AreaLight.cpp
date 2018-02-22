//
// Created by puscas on 03-03-2017.
//

#include "AreaLight.hpp"

using ::Components::AreaLight;
using ::MobileRT::Material;
using ::MobileRT::Sampler;
using ::MobileRT::Ray;
using ::MobileRT::Intersection;

AreaLight::AreaLight(Material radiance,
                     ::std::unique_ptr<Sampler> samplerPointLight,
                     const glm::vec3 pointA, const glm::vec3 pointB, const glm::vec3 pointC) noexcept :
        Light(::std::move(radiance)),
        triangle_{pointA, pointB, pointC},
        samplerPointLight_{::std::move(samplerPointLight)} {
}

glm::vec3 AreaLight::getPosition() noexcept {
    float R{samplerPointLight_->getSample()};
    float S{samplerPointLight_->getSample()};
    if (R + S >= 1.0f) {
        R = 1.0f - R;
        S = 1.0f - S;
    }
    return glm::vec3 {triangle_.pointA_.x + R * triangle_.AB_.x + S * triangle_.AC_.x,
                    triangle_.pointA_.y + R * triangle_.AB_.y + S * triangle_.AC_.y,
                    triangle_.pointA_.z + R * triangle_.AB_.z + S * triangle_.AC_.z};
}

void AreaLight::resetSampling() noexcept {
    samplerPointLight_->resetSampling();
}

Intersection AreaLight::intersect(Intersection intersection, Ray ray) const noexcept {
    const float dist {intersection.length_};
    intersection = triangle_.intersect(intersection, ::std::move(ray));
    if (intersection.length_ < dist) {
        intersection.material_ = &radiance_;
        return intersection;
    }
    return intersection;
}
