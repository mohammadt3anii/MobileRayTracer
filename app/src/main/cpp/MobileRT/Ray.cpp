//
// Created by Tiago on 16-10-2016.
//

#include "Ray.h"
#include "Constants.h"

using namespace MobileRT;

Ray::Ray() :
        maxDistance_(MAX_LENGTH),
        depth_(0)
{
}

Ray::Ray(const Point3D &orig, const Vector3D &dir, const float maxDist, const unsigned int depth) :
        origin_(orig),
        direction_(dir),
        maxDistance_(maxDist),
        depth_(depth)
{
    this->direction_.normalize();
}

void Ray::setRay (const float px, const float py, const float pz, const Point3D& org)
{
    this->direction_.setVect(px, py, pz);
    this->symDirection_.setVect(-px, -py, -pz);
    this->origin_ = org;
}