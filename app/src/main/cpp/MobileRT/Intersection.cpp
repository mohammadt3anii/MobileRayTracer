//
// Created by Tiago on 16-10-2016.
//

#include "Intersection.h"
#include "Constants.h"

using namespace MobileRT;

Intersection::Intersection () :
        length_(MAX_LENGTH),
        material_(nullptr)
{
}

void Intersection::setIntersection (const Point3D& point, const Vector3D& normal, const float length, const Material* m)
{
    this->point_ = point;
    this->normal_ = normal;
    this->length_ = length;
    this->material_ = m;
}

Vector3D& Intersection::getSymNormal()
{
    this->symNormal_.setVect(-this->normal_.x_, -this->normal_.y_, -this->normal_.z_);
    return this->symNormal_;
}