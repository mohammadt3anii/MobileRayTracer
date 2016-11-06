//
// Created by puscas on 16-10-2016.
//

#include "Vect.h"
#include <math.h>

using namespace MobileRT;

Vect::Vect ()
    : x(0.f), y(0.f), z(0.f)
{
}

Vect::Vect (const float& px, const float& py, const float& pz) :
    x(px), y(py), z(pz)
{
}

Vect::Vect (const Vect& v) :
    x(v.x), y(v.y), z(v.z)
{
}

 float Vect::normalize ()
 {
    float length = Vect::length();
    if (length!=0.f)
    {
        float inv_length = 1.f / length;
        x *= inv_length;
        y *= inv_length;
        z *= inv_length;
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
    return Vect(-x, -y, -z);
}

// dot product
float Vect::dot (const Vect& v) const
{
    return (x*v.x + y*v.y + z*v.z);
}

void Vect::mult (const float& f) {
    x *= f;
    y *= f;
    z *= f;
}

 void Vect::sub (const Vect& v)
 {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

float Vect::length ()
{
    return static_cast<float>(sqrt(Vect::dot(*this)));
}

void Vect::add (const Vect& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

// dot product
float Vect::not_dot (const Point& v) const
{
    return v.multiply(*this).sumCoordenates();
}


Vect Vect::operator*(const float& value) const
{
    return Vect(x*value, y*value, z*value);
}