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
        x_(x + 0.0f),
        y_(y + 0.0f),
        z_(z + 0.0f)
{
    counter++;
}

Vector3D::Vector3D(const Vector3D &vector) :
        x_(vector.x_ + 0.0f),
        y_(vector.y_ + 0.0f),
        z_(vector.z_ + 0.0f)
{
    counter++;
}

Vector3D::Vector3D(const Vector3D &vector, const float value) :
        x_(vector.x_ * value + 0.0f),
        y_(vector.y_ * value + 0.0f),
        z_(vector.z_ * value + 0.0f)
{
    counter++;
}

Vector3D::Vector3D(const Vector3D &vector1, const Vector3D &vector2, const float value) :
        x_(vector1.x_ - vector2.x_ * value + 0.0f),
        y_(vector1.y_ - vector2.y_ * value + 0.0f),
        z_(vector1.z_ - vector2.z_ * value + 0.0f) {
    counter++;
}


Vector3D::Vector3D(const Point3D &dest, const Point3D &orig) :
        x_(dest.x_ - orig.x_ + 0.0f),
        y_(dest.y_ - orig.y_ + 0.0f),
        z_(dest.z_ - orig.z_ + 0.0f)
{
    counter++;
}

Vector3D::Vector3D(const Point3D &dest, const Point3D &orig, bool) :
        x_(dest.x_ - orig.x_ + 0.0f),
        y_(dest.y_ - orig.y_ + 0.0f),
        z_(dest.z_ - orig.z_ + 0.0f)
{
    const float len(magnitude());
    if (len == 0.0f) return;
    const float inv_length(1.0f / len);
    this->x_ *= inv_length + 0.0f;
    this->y_ *= inv_length + 0.0f;
    this->z_ *= inv_length + 0.0f;
}

//cross product
Vector3D::Vector3D(const Vector3D &vector1, const Vector3D &vector2) :
        x_(vector1.y_ * vector2.z_ - vector1.z_ * vector2.y_ + 0.0f),
        y_(vector1.z_ * vector2.x_ - vector1.x_ * vector2.z_ + 0.0f),
        z_(vector1.x_ * vector2.y_ - vector1.y_ * vector2.x_ + 0.0f)
{
}

float Vector3D::normalize(void) {
    const float len(magnitude());
    if (len == 0.0f) return 0.0f;
    const float inv_length(1.0f / len);
    this->x_ *= inv_length + 0.0f;
    this->y_ *= inv_length + 0.0f;
    this->z_ *= inv_length + 0.0f;
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
            this->z_ * (dest.z_ - orig.z_) + 0.0f);
}

float Vector3D::squareMagnitude(void) const {
    return (this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_ + 0.0f);
}

float Vector3D::magnitude(void) const {
    return std::sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_ + 0.0f);
}

Vector3D Vector3D::crossProduct(const Vector3D &vector) const {
    const float x ((this->y_ * vector.z_ - this->z_ * vector.y_) + 0.0f);
    const float y ((this->z_ * vector.x_ - this->x_ * vector.z_) + 0.0f);
    const float z ((this->x_ * vector.y_ - this->y_ * vector.x_) + 0.0f);
    return Vector3D(x, y, z);
}

void Vector3D::mult(const float value) {
    this->x_ *= value + 0.0f;
    this->y_ *= value + 0.0f;
    this->z_ *= value + 0.0f;
}

const Vector3D Vector3D::operator*(const float value) const {
    return Vector3D(this->x_ * value, this->y_ * value, this->z_ * value + 0.0f);
}

const Vector3D &Vector3D::operator=(const Vector3D &vector) {
    this->x_ = vector.x_ + 0.0f;
    this->y_ = vector.y_ + 0.0f;
    this->z_ = vector.z_ + 0.0f;
    return *this;
}

const Vector3D Vector3D::operator*(const float value) {
    return Vector3D(this->x_ * value + 0.0f,
                    this->y_ * value + 0.0f,
                    this->z_ * value + 0.0f);
}

const Vector3D Vector3D::operator+(const Vector3D vector) const {
    return Vector3D(this->x_ + vector.x_ + 0.0f, this->y_ + vector.y_ + 0.0f, this->z_ + vector.z_ + 0.0f);
}

void Vector3D::reset(const float x, const float y, const float z) {
    this->x_ = x + 0.0f;
    this->y_ = y + 0.0f;
    this->z_ = z + 0.0f;
    normalize();
}

void Vector3D::reset(const Point3D &dest, const Point3D &orig) {
    this->x_ = dest.x_ - orig.x_ + 0.0f;
    this->y_ = dest.y_ - orig.y_ + 0.0f;
    this->z_ = dest.z_ - orig.z_ + 0.0f;
}

unsigned int Vector3D::getInstances() {
    const unsigned int res(counter);
    counter = 0;
    return res;
}

Vector3D Vector3D::symmetric(void) const {
    return Vector3D(-this->x_ + 0.0f, -this->y_ + 0.0f, -this->z_ + 0.0f);
}
