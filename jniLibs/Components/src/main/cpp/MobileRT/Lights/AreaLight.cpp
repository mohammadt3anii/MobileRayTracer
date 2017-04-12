//
// Created by puscas on 03-03-2017.
//

#include "AreaLight.h"

using namespace MobileRT;

AreaLight::AreaLight(const RGB &radiance, Sampler &samplerPointLight,
                     const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) :
        Light(radiance),
        Triangle(pointA, pointB, pointC),
        samplerPointLight_(samplerPointLight)
{
}

const Point3D AreaLight::getPosition(void)
{
        //float R(samplerPointLight_.getSample(0));
        //float S(samplerPointLight_.getSample(0));
        float R (static_cast<float>(rand()) / (RAND_MAX));
        float S (static_cast<float>(rand()) / (RAND_MAX));
        if (R + S >= 1.0f) {
                R = 1.0f - R;
                S = 1.0f - S;
        }
        const float x(pointA_.x_ + R * AB_.x_ + S * AC_.x_);
        const float y(pointA_.y_ + R * AB_.y_ + S * AC_.y_);
        const float z(pointA_.z_ + R * AB_.z_ + S * AC_.z_);
        Point3D position(x, y, z);
        return position;
}

void AreaLight::resetSampling(void) {
        samplerPointLight_.resetSampling();
}
