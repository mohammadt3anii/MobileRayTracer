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

void Intersection::setIntersection (const Point& point, const Vect& normal, const float length, const Material* m)
{
    point_ = point;
    normal_ = normal;
    length_ = length;
    material_ = m;
}

Vect& Intersection::getSymNormal()
{
    symNormal_.setVect(-normal_.x_, -normal_.y_, -normal_.z_);
    return this->symNormal_;
}