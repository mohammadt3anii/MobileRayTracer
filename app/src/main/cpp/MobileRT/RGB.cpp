//
// Created by puscas on 16-10-2016.
//
#include "RGB.h"

using namespace MobileRT;

RGB::RGB () :
    R(0.0f), G(0.0f), B(0.0f)
{
}

RGB::RGB (const float r, const float g, const float b) :
    R(r), G(g), B(b)
{
}

RGB::RGB (const RGB& rgb) :
    R(rgb.R), G(rgb.G), B(rgb.B)
{
}

bool RGB::isZero () const
{
    if ((R > 0.0f) || (G > 0.0f) || (B > 0.0f))
        return false;
    return true;
}

void RGB::add (const RGB& rgb)
{
    R += rgb.R;
    G += rgb.G;
    B += rgb.B;
}

void RGB::mult (const RGB& rgb)
{
    R *= rgb.R;
    G *= rgb.G;
    B *= rgb.B;
}

void RGB::mult (const float f)
{
    R *= f;
    G *= f;
    B *= f;
}