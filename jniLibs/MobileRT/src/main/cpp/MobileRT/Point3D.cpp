//
// Created by Tiago on 16-10-2016.
//

#include "Point3D.h"

using namespace MobileRT;

static unsigned int counter(0u);

Point3D::Point3D(void) :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f) {
    counter++;
}

Point3D::Point3D(const float px, const float py, const float pz) :
        x_(px),
        y_(py),
        z_(pz) {
    counter++;
}

Point3D::Point3D(const Point3D &p) :
        x_(p.x_),
        y_(p.y_),
        z_(p.z_) {
    counter++;
}

Point3D::Point3D(const Point3D &point, const Vector3D &vector, const float value) :
        x_(vector.x_ * value + point.x_),
        y_(vector.y_ * value + point.y_),
        z_(vector.z_ * value + point.z_) {
}

const Vector3D Point3D::operator-(const Point3D &point) const {
    return Vector3D(this->x_ - point.x_,
                    this->y_ - point.y_,
                    this->z_ - point.z_);
}

const Point3D Point3D::operator+(const Vector3D &vector) const {
    return Point3D(this->x_ + vector.x_,
                   this->y_ + vector.y_,
                   this->z_ + vector.z_);
}

void Point3D::operator=(const Point3D &point3D) {
    this->x_ = point3D.x_;
    this->y_ = point3D.y_;
    this->z_ = point3D.z_;
}

unsigned int Point3D::getInstances() {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
