//
// Created by puscas on 16-10-2016.
//

#include "Point.hpp"
#include "Vect.hpp"

using namespace MobileRT;

Point::Point () :
    x_(0.0f),
    y_(0.0f),
    z_(0.0f)
{
}

Point::Point (const float px, const float py, const float pz) :
    x_(px),
    y_(py),
    z_(pz)
{
}

Point::Point (const Point& p) :
    x_(p.x_),
    y_(p.y_),
    z_(p.z_)
{
}

Point Point::square() const
{
    return Point(this->x_ * this->x_, this->y_ * this->y_, this->z_ * this->z_);
}

Point Point::multiply(const Vect& vector) const
{
    return Point(this->x_ * vector.x, this->y_ * vector.y, this->z_ * vector.z);
}

float Point::sumCoordenates() const
{
    return this->x_ + this->y_ + this->z_;
}


Point Point::operator+(const Vect& vector) const
{
    return Point(this->x_ + vector.x, this->y_ + vector.y, this->z_ + vector.z);
}

Vect Point::operator-(const Point& point) const
{
    return Vect(this->x_ - point.x_, this->y_ - point.y_, this->z_ - point.z_);
}

Point Point::operator*(const float scale) const
{
    return Point(this->x_ * scale, this->y_ * scale, this->z_ * scale);
}