//
// Created by puscas on 03-03-2017.
//

#include "AreaLight.h"

using namespace MobileRT;

AreaLight::AreaLight(const Material &radiance, Sampler &samplerPointLight,
                     const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) :
        Light(radiance),
        Triangle(pointA, pointB, pointC),
        samplerPointLight_(samplerPointLight)
{
}

const Point3D AreaLight::getPosition(void)
{
    float R(samplerPointLight_.getSample(0));
    float S(samplerPointLight_.getSample(0));
    samplerPointLight_.getSample(0);
    //LOG("R = %f, S = %f", double(R), double(S));
    if (R + S >= 1.0f) {
        R = 1.0f - R;
        S = 1.0f - S;
    }
    const float x(pointA_.x_ + R * AB_.x_ + S * AC_.x_);
    const float y(pointA_.y_ + R * AB_.y_ + S * AC_.y_);
    const float z(pointA_.z_ + R * AB_.z_ + S * AC_.z_);
    Point3D position(x + 0.0f, y + 0.0f, z + 0.0f);
    return position;
}

void AreaLight::resetSampling(void) {
        samplerPointLight_.resetSampling();
}

bool AreaLight::intersect(Intersection &intersection, const Ray &ray, const Material &) const {
        return Triangle::intersect(intersection, ray, this->radiance_);
}
