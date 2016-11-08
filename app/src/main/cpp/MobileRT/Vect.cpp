//
// Created by puscas on 16-10-2016.
//

#include "Vect.h"
#include <math.h>

using namespace MobileRT;

Vect::Vect () :
    x(0.0f),
    y(0.0f),
    z(0.0f)
{
}

Vect::Vect (const float& px, const float& py, const float& pz) :
    x(px),
    y(py),
    z(pz)
{
}

Vect::Vect (const Vect& v) :
    x(v.x),
    y(v.y),
    z(v.z)
{
}

 float Vect::normalize ()
 {
    const float length = Vect::length();
    if (length != 0.0f)
    {
        float inv_length = 1.0f / length;
        this->x *= inv_length;
        this->y *= inv_length;
        this->z *= inv_length;
    }
    return length;
}

Vect Vect::returnNormalized() const
{
    Vect normalized(*this);
    normalized.normalize();
    return normalized;
}

// symetric vector
Vect Vect::symmetric () const
{
    return Vect(-this->x, -this->y, -this->z);
}

// dot product
float Vect::dot (const Vect& v) const
{
    return (this->x * v.x + this->y * v.y + this->z * v.z);
}

void Vect::mult (const float& f) {
    this->x *= f;
    this->y *= f;
    this->z *= f;
}

 void Vect::sub (const Vect& v)
 {
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
}

float Vect::length ()
{
    return sqrt(Vect::dot(*this));
}

void Vect::add (const Vect& v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
}

// dot product
float Vect::not_dot (const Point& v) const
{
    return v.multiply(*this).sumCoordenates();
}


Vect Vect::operator*(const float& value) const
{
    return Vect(this->x * value, this->y * value, this->z * value);
}