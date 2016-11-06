//
// Created by puscas on 16-10-2016.
//

#include "Ray.h"
#include "Constants.h"

using namespace MobileRT;

Ray::Ray () :
    max_T(MAX_LENGTH),depth(0), dir(Vect(0.0f, 0.0f, 1.0f))
{
}

Ray::Ray (const Point& porig, const Vect& pdir) :
        orig(porig),dir(pdir),max_T(MAX_LENGTH),depth(0)
{
    dir.normalize();
}

Ray::Ray (const Point& porig, const Vect& pdir, const float& pmax_T) :
    orig(porig),dir(pdir),max_T(pmax_T),depth(0)
{
    dir.normalize();
}

Ray::Ray (const Point& porig, const Vect& pdir, const float& pmax_T, const int& d) :
    orig(porig),dir(pdir),max_T(pmax_T),depth(d)
{
    dir.normalize();
}