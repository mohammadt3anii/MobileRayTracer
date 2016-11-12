//
// Created by puscas on 16-10-2016.
//
#include "RGB.hpp"

using namespace MobileRT;

RGB::RGB () :
    R(0.0f),
    G(0.0f),
    B(0.0f)
{
}

RGB::RGB (const float r, const float g, const float b) :
    R(r),
    G(g),
    B(b)
{
}

RGB::RGB (const RGB& rgb) :
    R(rgb.R),
    G(rgb.G),
    B(rgb.B)
{
}

bool RGB::isZero () const
{
    return (((this->R > 0.0f) || (this->G > 0.0f) || (this->B > 0.0f)) == true)? false : true;
}

void RGB::add (const RGB& rgb)
{
    this->R += rgb.R;
    this->G += rgb.G;
    this->B += rgb.B;
}

void RGB::mult (const RGB& rgb)
{
    this->R *= rgb.R;
    this->G *= rgb.G;
    this->B *= rgb.B;
}

void RGB::mult (const float f)
{
    this->R *= f;
    this->G *= f;
    this->B *= f;
}

void RGB::setRGB ()
{
    this->R = 0.0f;
    this->G = 0.0f;
    this->B = 0.0f;
}

void RGB::setRGB (const float r, const float g, const float b)
{
    this->R = r;
    this->G = g;
    this->B = b;
}