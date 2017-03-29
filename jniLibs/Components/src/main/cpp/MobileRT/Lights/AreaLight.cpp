//
// Created by puscas on 03-03-2017.
//

#include "AreaLight.h"

using namespace MobileRT;

AreaLight::AreaLight(const RGB &radiance, const Point3D &position, Sampler &sampler,
                     const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) :
        Light(radiance, position),
        Triangle(pointA, pointB, pointC),
        sampler_(sampler)
{
}

Point3D AreaLight::getPosition(const unsigned int, const unsigned int)
{
        static unsigned int counter(0);
        ++counter;
        //if (this->sampler_.domainSize_ <= 1) return this->position_;
        //const float rX (sampler_.getSample(0));
        //const float rZ (sampler_.getSample(0));
        //const float deviationX (r * 2.0f - 1.0f);
        //const float deviationZ (r * 2.0f - 1.0f);
        //const float domainSize (std::sqrt(this->sampler_.domainSize_));
        float R(sampler_.getSample(0));
        float S(sampler_.getSample(0));
        if (R + S >= 1) {
                R = 1 - R;
                S = 1 - S;
        }
        const float x(pointA_.x_ + R * AB_.x_ + S * AC_.x_);
        const float y(pointA_.y_ + R * AB_.y_ + S * AC_.y_);
        const float z(pointA_.z_ + R * AB_.z_ + S * AC_.z_);
        /*const float rX (i / domainSize + r1 * 0.5f / domainSize);
        const float rZ (j / domainSize + r2 * 0.5f / domainSize);
        const float deviationX ((rX + 0.5f / domainSize) * 2.0f - 1.0f);
        const float deviationZ ((rZ + 0.5f / domainSize) * 2.0f - 1.0f);
        const float x (position_.x_+sizeX_*deviationX);
        const float y (position_.y_);
        const float z (position_.z_+sizeZ_*deviationZ);
        LOG("i=%u, j=%u, r1=%f, r2=%f, rX=%f, rZ=%f, deviationX=%f, deviationZ=%f domainSize=%f",
                i, j, double(r1), double(r2), double(rX), double (rZ),
                double(deviationX), double(deviationZ), double(domainSize));*/
        //LOG("X=%f, Y=%f, Z=%f", double(x), double(y), double(z));
        //if (counter <= 2) LOG("domainSize = %llu", sampler_.domainSize_);
        /*ASSERT(r1, >=, -1.0f);
        ASSERT(r1, <=, 1.0f);
        ASSERT(r2, >=, -1.0f);
        ASSERT(r2, <=, 1.0f);
        ASSERT(deviationX, >=, -1.0f);
        ASSERT(deviationX, <=, 1.0f);
        ASSERT(deviationZ, >=, -1.0f);
        ASSERT(deviationZ, <=, 1.0f);
        ASSERT(x, <=, position_.x_+sizeX_);
        ASSERT(x, >=, position_.x_-sizeX_);
        ASSERT(z, <=, position_.z_+sizeZ_);
        ASSERT(z, >=, position_.z_-sizeZ_);*/
        Point3D position(x, y, z);
        return position;
}

void AreaLight::resetSampling(void) {
        sampler_.resetSampling();
}
