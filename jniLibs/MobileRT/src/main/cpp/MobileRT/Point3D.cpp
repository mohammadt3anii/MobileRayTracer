//
// Created by Tiago on 16-10-2016.
//

#include "Point3D.hpp"

using MobileRT::Point3D;
using MobileRT::Vector3D;

static unsigned int counter(0u);

Point3D::Point3D(const float x, const float y, const float z) noexcept :
        x_(x),
        y_(y),
        z_(z) {
    counter++;
}

Point3D::Point3D(const Point3D &point) noexcept :
        x_(point.x_),
        y_(point.y_),
        z_(point.z_) {
    counter++;
}

Point3D::Point3D(const Point3D &point, const Vector3D &vector, const float value) noexcept :
        x_(vector.x_ * value + point.x_),
        y_(vector.y_ * value + point.y_),
        z_(vector.z_ * value + point.z_) {
}

const Vector3D Point3D::operator-(const Point3D &point) const noexcept {
    return Vector3D(this->x_ - point.x_,
                    this->y_ - point.y_,
                    this->z_ - point.z_);
}

const Point3D Point3D::operator+(const Vector3D &vector) const noexcept {
    return Point3D(this->x_ + vector.x_,
                   this->y_ + vector.y_,
                   this->z_ + vector.z_);
}

void Point3D::operator=(const Point3D &point3D) noexcept {
    this->x_ = point3D.x_;
    this->y_ = point3D.y_;
    this->z_ = point3D.z_;
}

unsigned int Point3D::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
