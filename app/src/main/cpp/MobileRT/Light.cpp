//
// Created by puscas on 16-10-2016.
//

#include "Light.h"
#include "RGB.h"

using namespace MobileRT;

Light::Light (const RGB& prad, const Point& ppos) :
    rad(prad), pos(ppos)
{
}