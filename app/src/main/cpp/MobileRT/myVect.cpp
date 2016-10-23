//
// Created by puscas on 16-10-2016.
//

#include "myVect.h"
#include <math.h>

using namespace MobileRT;

myVect::myVect ()
    : x(0.f), y(0.f), z(0.f)
{
}

myVect::myVect (const float px, const float py, const float pz) :
    x(px), y(py), z(pz)
{
}

myVect::myVect (const myVect& v) :
    x(v.x), y(v.y), z(v.z)
{
}

 float myVect::normalize ()
 {
    float length = myVect::length();
    if (length!=0.f)
    {
        float inv_length = 1.f / length;
        x *= inv_length;
        y *= inv_length;
        z *= inv_length;
    }
    return length;
}

// symetric vector
myVect myVect::symmetric () const
{
    return myVect(-x, -y, -z);
}

// dot product
float myVect::dot (const myVect& v)
{
    return (x*v.x + y*v.y + z*v.z);
}

void myVect::mult (const float f) {
    x *= f;
    y *= f;
    z *= f;
}

 void myVect::sub (const myVect& v)
 {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

float myVect::length ()
{
    return static_cast<float>(sqrt(myVect::dot(*this)));
}

void myVect::add (const myVect& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

// dot product
float myVect::dot (const myPoint& v)
{
    return (x*v.x + y*v.y + z*v.z);
}