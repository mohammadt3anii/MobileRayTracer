//
// Created by Tiago on 16-10-2016.
//

#include "Vect.h"
#include <cmath>

using namespace MobileRT;

Vect::Vect () :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f)
{
}

Vect::Vect (const float px, const float py, const float pz) :
        x_(px),
        y_(py),
        z_(pz)
{
}

Vect::Vect (const Vect& v) :
        x_(v.x_),
        y_(v.y_),
        z_(v.z_)
{
}

float Vect::normalize() {
    const float length = Vect::length();
    if (length != 0.0f)
    {
        float inv_length = 1.0f / length;
        this->x_ *= inv_length;
        this->y_ *= inv_length;
        this->z_ *= inv_length;
    }
    return length;
}

Vect Vect::returnNormalized () const
{
    Vect normalized(*this);
    normalized.normalize();
    return normalized;
}

// symetric vector
Vect Vect::symmetric () const
{
    return Vect(-this->x_, -this->y_, -this->z_);
}

// dot product Algebraic
float Vect::dot (const Vect& v) const
{
    return (this->x_ * v.x_ + this->y_ * v.y_ + this->z_ * v.z_);
}

// dot product Algebraic
float Vect::dot() const {
    return (this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

void Vect::mult (const float f)
{
    this->x_ *= f;
    this->y_ *= f;
    this->z_ *= f;
}

 void Vect::sub (const Vect& v)
 {
     this->x_ -= v.x_;
     this->y_ -= v.y_;
     this->z_ -= v.z_;
}

float Vect::length ()
{
    return std::sqrt(dot());
}

Vect Vect::operator* (const float value) const
{
    return Vect(this->x_ * value, this->y_ * value, this->z_ * value);
}

void Vect::setVect (const float px, const float py, const float pz)
{
    this->x_ = px;
    this->y_ = py;
    this->z_ = pz;
    normalize();
}

void Vect::setVect (const Point& dest, const Point& orig)
{
    this->x_ = dest.x_ - orig.x_;
    this->y_ = dest.y_ - orig.y_;
    this->z_ = dest.z_ - orig.z_;
}