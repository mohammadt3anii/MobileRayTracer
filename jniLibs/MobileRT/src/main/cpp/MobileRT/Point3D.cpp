//
// Created by Tiago on 16-10-2016.
//

#include "Point3D.h"

using namespace MobileRT;

static unsigned int counter = 0;

Point3D::Point3D(void) :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f)
{
    counter++;
}

Point3D::Point3D(const float px, const float py, const float pz) :
        x_(px + 0.0f),
        y_(py + 0.0f),
        z_(pz + 0.0f)
{
    counter++;
}

Point3D::Point3D(const Point3D &p) :
        x_(p.x_ + 0.0f),
        y_(p.y_ + 0.0f),
        z_(p.z_ + 0.0f)
{
    counter++;
}

Point3D::Point3D(Point3D &p) :
        x_(p.x_ + 0.0f),
        y_(p.y_ + 0.0f),
        z_(p.z_ + 0.0f)
{
    counter++;
}

Point3D::Point3D(const Point3D &point, const Vector3D &vector, const float value) :
        x_(vector.x_ * value + point.x_ + 0.0f),
        y_(vector.y_ * value + point.y_ + 0.0f),
        z_(vector.z_ * value + point.z_ + 0.0f)
{
}

const Vector3D Point3D::operator-(const Point3D &point) const {
    const float x(this->x_ - point.x_);
    const float y(this->y_ - point.y_);
    const float z(this->z_ - point.z_);
    return Vector3D(x + 0.0f, y + 0.0f, z + 0.0f);
}

const Point3D Point3D::operator+(const Vector3D &vector) const {
    const float x(this->x_ + vector.x_);
    const float y(this->y_ + vector.y_);
    const float z(this->z_ + vector.z_);
    Point3D res(x + 0.0f, y + 0.0f, z + 0.0f);
    return res;
}

void Point3D::operator=(const Point3D &point3D) {
    this->x_ = point3D.x_ + 0.0f;
    this->y_ = point3D.y_ + 0.0f;
    this->z_ = point3D.z_ + 0.0f;
}

unsigned int Point3D::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}

void Point3D::reset(const Point3D &point, const Vector3D &vector, const float value) {
    this->x_ = (vector.x_ * value + point.x_) + 0.0f;
    this->y_ = (vector.y_ * value + point.y_) + 0.0f;
    this->z_ = (vector.z_ * value + point.z_) + 0.0f;
}
