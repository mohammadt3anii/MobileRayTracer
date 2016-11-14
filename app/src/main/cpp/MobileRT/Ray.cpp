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

Ray::Ray(const float x, const float y, const float z, Point3D origin) :
        origin_(origin),
        direction_(x, y, z),
        maxDistance_(MAX_LENGTH),
        depth_(0),
        symDirection_(-x, -y, -z)
{
    this->direction_.normalize();
}

Ray::Ray(const Point3D &orig, const Vector3D &dir, const float maxDist, const unsigned int depth) :
        origin_(orig),
        direction_(dir),
        maxDistance_(maxDist),
        depth_(depth)
{
    this->direction_.normalize();
}

void Ray::recycle (const float x, const float y, const float z, const Point3D& origin)
{
    this->direction_.recycle(x, y, z);
    this->symDirection_.recycle(-x, -y, -z);
    this->origin_ = origin;
}