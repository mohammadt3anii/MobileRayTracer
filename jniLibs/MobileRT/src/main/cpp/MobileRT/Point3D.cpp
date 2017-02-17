//
// Created by Tiago on 16-10-2016.
//

#include "Point3D.h"

using namespace MobileRT;

Point3D::Point3D() :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f) {
}

Point3D::Point3D(const float px, const float py, const float pz) :
        x_(px),
        y_(py),
        z_(pz) {
}

Point3D::Point3D(const Point3D &p) :
        x_(p.x_),
        y_(p.y_),
        z_(p.z_) {
}

Point3D Point3D::operator+(const Vector3D &vector) const {
    return Point3D(this->x_ + vector.x_, this->y_ + vector.y_, this->z_ + vector.z_);
}

Vector3D Point3D::operator-(const Point3D &point) const {
    return Vector3D(this->x_ - point.x_, this->y_ - point.y_, this->z_ - point.z_);
}

Point3D &Point3D::operator=(const Point3D &point3D) {
    this->x_ = point3D.x_;
    this->y_ = point3D.y_;
    this->z_ = point3D.z_;
    return *this;
}
