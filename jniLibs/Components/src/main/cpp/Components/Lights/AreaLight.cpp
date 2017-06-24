//
// Created by puscas on 03-03-2017.
//

#include "AreaLight.h"

using namespace Components;

AreaLight::AreaLight(const Material &radiance, Sampler &samplerPointLight,
                     const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) :
        Light(radiance),
        Triangle(pointA, pointB, pointC),
        samplerPointLight_(samplerPointLight) {
}

const Point3D AreaLight::getPosition(void) {
    float R(samplerPointLight_.getSample(0));
    float S(samplerPointLight_.getSample(0));
    if (R + S >= 1.0f) {
        R = 1.0f - R;
        S = 1.0f - S;
    }
    Point3D position(pointA_.x_ + R * AB_.x_ + S * AC_.x_,
                     pointA_.y_ + R * AB_.y_ + S * AC_.y_,
                     pointA_.z_ + R * AB_.z_ + S * AC_.z_);
    return position;
}

void AreaLight::resetSampling(void) {
    samplerPointLight_.resetSampling();
}

bool AreaLight::intersect(Intersection &intersection, const Ray &ray, const Material &) const {
    return Triangle::intersect(intersection, ray, this->radiance_);
}
