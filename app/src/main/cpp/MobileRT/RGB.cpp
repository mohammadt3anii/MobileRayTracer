//
// Created by Tiago on 16-10-2016.
//
#include "RGB.h"

using namespace MobileRT;

RGB::RGB () :
        R_(0.0f),
        G_(0.0f),
        B_(0.0f)
{
}

RGB::RGB (const float r, const float g, const float b) :
        R_(r),
        G_(g),
        B_(b)
{
}

RGB::RGB (const RGB& rgb) :
        R_(rgb.R_),
        G_(rgb.G_),
        B_(rgb.B_)
{
}

bool RGB::isZero () const
{
    return (((this->R_ > 0.0f) || (this->G_ > 0.0f) || (this->B_ > 0.0f)) == true) ? false : true;
}

void RGB::add (const RGB& rgb)
{
    this->R_ += rgb.R_;
    this->G_ += rgb.G_;
    this->B_ += rgb.B_;
}

void RGB::mult (const RGB& rgb)
{
    this->R_ *= rgb.R_;
    this->G_ *= rgb.G_;
    this->B_ *= rgb.B_;
}

void RGB::mult (const float f)
{
    this->R_ *= f;
    this->G_ *= f;
    this->B_ *= f;
}

void RGB::setRGB ()
{
    this->R_ = 0.0f;
    this->G_ = 0.0f;
    this->B_ = 0.0f;
}

void RGB::setRGB (const float r, const float g, const float b)
{
    this->R_ = r;
    this->G_ = g;
    this->B_ = b;
}