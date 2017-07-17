//
// Created by Tiago on 16-10-2016.
//

#include "Vector3D.hpp"

using MobileRT::Vector3D;
using MobileRT::Point3D;

static unsigned int counter(0u);

Vector3D::Vector3D(const float x, const float y, const float z) noexcept :
        x_(x),
        y_(y),
        z_(z),
		magnitude_(magnitude())
{
    counter++;
}

// Vector3D::Vector3D(const Vector3D &vector) noexcept :
//         x_(vector.x_),
//         y_(vector.y_),
//         z_(vector.z_),
// 		magnitude_(magnitude())
// {
//     const float len(magnitude_);
//     const float inv_length(len == 0.0f ? 1.0f : 1.0f / len);
//     this->x_ *= inv_length;
//     this->y_ *= inv_length;
//     this->z_ *= inv_length;
//     counter++;
// }

Vector3D::Vector3D(const Vector3D &vector, const float value) noexcept :
        x_(vector.x_ * value),
        y_(vector.y_ * value),
        z_(vector.z_ * value),
		magnitude_(magnitude())
{
    counter++;
}

Vector3D::Vector3D(const Vector3D &vector1, const Vector3D &vector2, const float value) noexcept :
        x_(vector1.x_ - vector2.x_ * value),
        y_(vector1.y_ - vector2.y_ * value),
        z_(vector1.z_ - vector2.z_ * value),
		magnitude_(magnitude())
{
    const float len(magnitude_);
    const float inv_length(len == 0.0f ? 1.0f : 1.0f / len);
    this->x_ *= inv_length;
    this->y_ *= inv_length;
    this->z_ *= inv_length;
    counter++;
}


Vector3D::Vector3D(const Point3D &dest, const Point3D &orig) noexcept :
        x_(dest.x_ - orig.x_),
        y_(dest.y_ - orig.y_),
        z_(dest.z_ - orig.z_),
		magnitude_(magnitude())
{
    counter++;
}

Vector3D::Vector3D(const Point3D &dest, const Point3D &orig, bool /*normalize*/) noexcept :
        x_(dest.x_ - orig.x_),
        y_(dest.y_ - orig.y_),
        z_(dest.z_ - orig.z_),
		magnitude_(magnitude())
{
    const float inv_length(magnitude_ == 0.0f ? 1.0f : 1.0f / magnitude_);
    this->x_ *= inv_length;
    this->y_ *= inv_length;
    this->z_ *= inv_length;

    counter++;
}

//cross product
Vector3D::Vector3D(const Vector3D &vector1, const Vector3D &vector2) noexcept :
        x_(vector1.y_ * vector2.z_ - vector1.z_ * vector2.y_),
        y_(vector1.z_ * vector2.x_ - vector1.x_ * vector2.z_),
        z_(vector1.x_ * vector2.y_ - vector1.y_ * vector2.x_),
		magnitude_(magnitude())
{
}

float Vector3D::normalize() noexcept {
    const float len(magnitude());
    const float inv_length(len == 0.0f ? 1.0f : 1.0f / len);
    this->x_ *= inv_length;
    this->y_ *= inv_length;
    this->z_ *= inv_length;
    return len;
}

float Vector3D::dotProduct(const Vector3D &vector) const noexcept {
    return (this->x_ * vector.x_ + this->y_ * vector.y_ + this->z_ * vector.z_);
}

float Vector3D::dotProduct(const Point3D &dest, const Point3D &orig) const noexcept {
    return (this->x_ * (dest.x_ - orig.x_) +
            this->y_ * (dest.y_ - orig.y_) +
            this->z_ * (dest.z_ - orig.z_));
}

float Vector3D::squareMagnitude() const noexcept {
    return (this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

float Vector3D::magnitude() const noexcept {
    return std::sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

const Vector3D Vector3D::crossProduct(const Vector3D &vector) const noexcept {
    const float x(this->y_ * vector.z_ - this->z_ * vector.y_);
    const float y(this->z_ * vector.x_ - this->x_ * vector.z_);
    const float z(this->x_ * vector.y_ - this->y_ * vector.x_);
    const float length(
            1.0f / std::sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_));

    return Vector3D(x * length, y * length, z * length);
}

void Vector3D::mult(const float value) noexcept {
    this->x_ *= value;
    this->y_ *= value;
    this->z_ *= value;
}

const Vector3D Vector3D::operator*(const float value) const noexcept {
    return Vector3D(this->x_ * value, this->y_ * value, this->z_ * value);
}

const Vector3D Vector3D::operator*(const float value) noexcept {
    return Vector3D(this->x_ * value,
                    this->y_ * value,
                    this->z_ * value);
}

const Point3D Vector3D::operator/(const float value) const noexcept {
    return Point3D(
            this->x_ / value,
            this->y_ / value,
            this->z_ / value);
}

const Vector3D Vector3D::operator+(const Vector3D &vector) const noexcept {
    return Vector3D(this->x_ + vector.x_, this->y_ + vector.y_, this->z_ + vector.z_);
}

unsigned int Vector3D::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}

const Vector3D Vector3D::symmetric() const noexcept {
    return Vector3D(-this->x_, -this->y_, -this->z_);
}
