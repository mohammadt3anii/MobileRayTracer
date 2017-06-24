//
// Created by Tiago on 16-10-2016.
//

#include "Vector3D.h"

using namespace MobileRT;

static unsigned int counter = 0;

Vector3D::Vector3D(void) :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f)
{
    counter++;
}

Vector3D::Vector3D(const float x, const float y, const float z) :
        x_(x),
        y_(y),
        z_(z)
{
    counter++;
}

Vector3D::Vector3D(const Vector3D &vector) :
        x_(vector.x_),
        y_(vector.y_),
        z_(vector.z_)
{
    counter++;
}

Vector3D::Vector3D(const Vector3D &vector, const float value) :
        x_(vector.x_ * value),
        y_(vector.y_ * value),
        z_(vector.z_ * value)
{
    counter++;
}

Vector3D::Vector3D(const Vector3D &vector1, const Vector3D &vector2, const float value) :
        x_(vector1.x_ - vector2.x_ * value),
        y_(vector1.y_ - vector2.y_ * value),
        z_(vector1.z_ - vector2.z_ * value) {
    counter++;
}


Vector3D::Vector3D(const Point3D &dest, const Point3D &orig) :
        x_(dest.x_ - orig.x_),
        y_(dest.y_ - orig.y_),
        z_(dest.z_ - orig.z_)
{
    counter++;
}

Vector3D::Vector3D(const Point3D &dest, const Point3D &orig, bool) :
        x_(dest.x_ - orig.x_),
        y_(dest.y_ - orig.y_),
        z_(dest.z_ - orig.z_)
{
    const float len(magnitude());
    if (len == 0.0f) return;
    const float inv_length(1.0f / len);
    this->x_ *= inv_length;
    this->y_ *= inv_length;
    this->z_ *= inv_length;
}

//cross product
Vector3D::Vector3D(const Vector3D &vector1, const Vector3D &vector2) :
        x_(vector1.y_ * vector2.z_ - vector1.z_ * vector2.y_),
        y_(vector1.z_ * vector2.x_ - vector1.x_ * vector2.z_),
        z_(vector1.x_ * vector2.y_ - vector1.y_ * vector2.x_)
{
}

float Vector3D::normalize(void) {
    const float len(magnitude());
    if (len == 0.0f) return 0.0f;
    const float inv_length(1.0f / len);
    this->x_ *= inv_length;
    this->y_ *= inv_length;
    this->z_ *= inv_length;
    return len;
}

const Vector3D Vector3D::returnNormalized(void) const {
    Vector3D normalized(*this);
    normalized.normalize();
    return normalized;
}

float Vector3D::dotProduct(const Vector3D &vector) const {
    return (this->x_ * vector.x_ + this->y_ * vector.y_ + this->z_ * vector.z_);
}

float Vector3D::dotProduct(const Point3D &dest, const Point3D &orig) const {
    return (this->x_ * (dest.x_ - orig.x_) +
            this->y_ * (dest.y_ - orig.y_) +
            this->z_ * (dest.z_ - orig.z_));
}

float Vector3D::squareMagnitude(void) const {
    return (this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

float Vector3D::magnitude(void) const {
    return std::sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

Vector3D Vector3D::crossProduct(const Vector3D &vector) const {
    const float x(this->y_ * vector.z_ - this->z_ * vector.y_);
    const float y(this->z_ * vector.x_ - this->x_ * vector.z_);
    const float z(this->x_ * vector.y_ - this->y_ * vector.x_);
    return Vector3D(x, y, z);
}

void Vector3D::mult(const float value) {
    this->x_ *= value;
    this->y_ *= value;
    this->z_ *= value;
}

const Vector3D Vector3D::operator*(const float value) const {
    return Vector3D(this->x_ * value, this->y_ * value, this->z_ * value);
}

const Vector3D &Vector3D::operator=(const Vector3D &vector) {
    this->x_ = vector.x_;
    this->y_ = vector.y_;
    this->z_ = vector.z_;
    return *this;
}

const Vector3D Vector3D::operator*(const float value) {
    return Vector3D(this->x_ * value,
                    this->y_ * value,
                    this->z_ * value);
}

const Vector3D Vector3D::operator+(const Vector3D vector) const {
    return Vector3D(this->x_ + vector.x_, this->y_ + vector.y_, this->z_ + vector.z_);
}

void Vector3D::reset(const float x, const float y, const float z) {
    this->x_ = x;
    this->y_ = y;
    this->z_ = z;
    normalize();
}

void Vector3D::reset(const Point3D &dest, const Point3D &orig) {
    this->x_ = dest.x_ - orig.x_;
    this->y_ = dest.y_ - orig.y_;
    this->z_ = dest.z_ - orig.z_;
}

unsigned int Vector3D::getInstances() {
    const unsigned int res(counter);
    counter = 0;
    return res;
}

Vector3D Vector3D::symmetric(void) const {
    return Vector3D(-this->x_, -this->y_, -this->z_);
}
