//
// Created by puscas on 16-10-2016.
//

#include "Ray.hpp"
#include "Constants.hpp"

using namespace MobileRT;

Ray::Ray (const Point& porig, const Vect& pdir) :
        orig(porig),
        dir(pdir),
        max_T(MAX_LENGTH),
        depth(0)
{
    this->dir.normalize();
}

Ray::Ray (const Point& porig, const Vect& pdir, const float pmax_T, const unsigned int d) :
    orig(porig),
    dir(pdir),
    max_T(pmax_T),
    depth(d)
{
    this->dir.normalize();
}

Ray::Ray () :
        max_T(MAX_LENGTH),
        depth(0)
{
}

void Ray::setRay (const float px, const float py, const float pz, const Point& org)
{
    this->dir.setVect(px, py, pz);
    this->orig = org;
}