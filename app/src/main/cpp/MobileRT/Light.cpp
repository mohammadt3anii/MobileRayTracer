//
// Created by puscas on 16-10-2016.
//

#include "Light.h"

using namespace MobileRT;

Light::Light (const RGB& prad, const Point& ppos) :
    rad_(prad),
    pos_(ppos)
{
}

const RGB& Light::getRad () const
{
    return this->rad_;
}

const Point& Light::getPos () const
{
    return this->pos_;
}