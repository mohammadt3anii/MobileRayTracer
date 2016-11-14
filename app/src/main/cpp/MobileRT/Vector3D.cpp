//
// Created by Tiago on 16-10-2016.
//

#include "Vector3D.h"
#include <cmath>

using namespace MobileRT;

Vector3D::Vector3D () :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f)
{
}

Vector3D::Vector3D (const float x, const float y, const float z) :
        x_(x),
        y_(y),
        z_(z)
{
}

Vector3D::Vector3D (const Vector3D& vector) :
        x_(vector.x_),
        y_(vector.y_),
        z_(vector.z_)
{
}

float Vector3D::normalize()
{
    const float length (Vector3D::length());
    if (length != 0.0f)
    {
        const float inv_length (1.0f / length);
        this->x_ *= inv_length;
        this->y_ *= inv_length;
        this->z_ *= inv_length;
    }
    return length;
}

Vector3D Vector3D::returnNormalized () const
{
    Vector3D normalized(*this);
    normalized.normalize();
    return normalized;
}

// symetric vector
Vector3D Vector3D::symmetric () const
{
    return Vector3D(-this->x_, -this->y_, -this->z_);
}

// dot product Algebraic
float Vector3D::dotProduct (const Vector3D& v) const
{
    return (this->x_ * v.x_ + this->y_ * v.y_ + this->z_ * v.z_);
}

// dot product Algebraic
float Vector3D::dot() const
{
    return (this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

void Vector3D::mult (const float value)
{
    this->x_ *= value;
    this->y_ *= value;
    this->z_ *= value;
}

 void Vector3D::sub (const Vector3D& vector)
 {
     this->x_ -= vector.x_;
     this->y_ -= vector.y_;
     this->z_ -= vector.z_;
}

float Vector3D::length ()
{
    return std::sqrt(dot());
}

Vector3D Vector3D::operator* (const float value) const
{
    return Vector3D(this->x_ * value, this->y_ * value, this->z_ * value);
}

void Vector3D::setVect (const float x, const float y, const float z)
{
    this->x_ = x;
    this->y_ = y;
    this->z_ = z;
    normalize();
}

void Vector3D::setVect (const Point3D& dest, const Point3D& orig)
{
    this->x_ = dest.x_ - orig.x_;
    this->y_ = dest.y_ - orig.y_;
    this->z_ = dest.z_ - orig.z_;
}