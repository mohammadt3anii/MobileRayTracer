//
// Created by puscas on 16-10-2016.
//

#include "Point.h"
#include "Vect.h"

using namespace MobileRT;

Point::Point () : x_(.0f), y_(.0f), z_(.0f)
{
}

Point::Point (const float& px, const float& py, const float& pz) :
    x_(px), y_(py), z_(pz) {
}

Point::Point (const Point& p) :
    x_(p.x_), y_(p.y_), z_(p.z_)
{
}

Point Point::square() const
{
    return Point(x_ * x_, y_ * y_, z_ * z_);
}

Point Point::multiply(const Vect& vector) const
{
    return Point(x_ * vector.x, y_ * vector.y, z_ * vector.z);
}

float Point::sumCoordenates() const
{
    return x_ + y_ + z_;
}


Point Point::operator+(const Vect& vector) const
{
    return Point(x_ + vector.x, y_ + vector.y, z_ + vector.z);
}

Vect Point::operator-(const Point& point) const
{
    return Vect(x_ - point.x_, y_ - point.y_, z_ - point.z_);
}

Point Point::operator*(const float& scale) const
{
    return Point(x_*scale, y_ *scale, z_*scale);
}