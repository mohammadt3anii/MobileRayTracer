//
// Created by Tiago on 16-10-2016.
//

#include "Vector3D.hpp"

using MobileRT::Vector3D;
using MobileRT::Point3D;
static unsigned counter {0};

namespace MobileRT {
  Vector3D operator/ (const int value, const Vector3D vector) noexcept {
    const float x {value / vector.x_};
    const float y {value / vector.y_};
    const float z {value / vector.z_};
    return Vector3D {x, y, z};
  }

  Vector3D operator/ (Vector3D vector, int value) noexcept {
    return Vector3D (vector.x_ / value, vector.y_ / value, vector.z_ / value);
  }
}//namespace MobileRT

float Vector3D::magnitude() const noexcept {

  return std::sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
}

Vector3D::Vector3D(const float x, const float y, const float z, const float magnitude) noexcept :
  x_ {x},
  y_ {y},
  z_ {z},
  magnitude_ {magnitude == 0.0f ? Vector3D::magnitude () : magnitude}
{
    counter++;
}

Vector3D::Vector3D (const Vector3D vector, const float value) noexcept :
  x_ {vector.x_ * value},
  y_ {vector.y_ * value},
  z_ {vector.z_ * value},
  magnitude_ {magnitude ()}
{
    counter++;
}

Vector3D::Vector3D (const Vector3D vector1, const Vector3D vector2, const float value) noexcept :
  x_ {vector1.x_ - vector2.x_ * value},
  y_ {vector1.y_ - vector2.y_ * value},
  z_ {vector1.z_ - vector2.z_ * value},
  magnitude_ {magnitude ()}
{
    this->x_ /= this->magnitude_;
    this->y_ /= this->magnitude_;
    this->z_ /= this->magnitude_;
    this->magnitude_ = 1.0f;
    counter++;
}

Vector3D::Vector3D (const Point3D dest, const Point3D orig) noexcept :
  x_ {dest.x_ - orig.x_},
  y_ {dest.y_ - orig.y_},
  z_ {dest.z_ - orig.z_},
  magnitude_ {magnitude ()}
{
    counter++;
}

Vector3D::Vector3D (const Point3D dest, const Point3D orig, const bool /*normalize*/) noexcept :
  x_ {dest.x_ - orig.x_},
  y_ {dest.y_ - orig.y_},
  z_ {dest.z_ - orig.z_},
  magnitude_ {magnitude ()}
{
  this->x_ /= this->magnitude_;
  this->y_ /= this->magnitude_;
  this->z_ /= this->magnitude_;
  this->magnitude_ = 1.0f;
  counter++;
}

//cross product
Vector3D::Vector3D (const Vector3D vector1, const Vector3D vector2) noexcept :
  x_ {vector1.y_ * vector2.z_ - vector1.z_ * vector2.y_},
  y_ {vector1.z_ * vector2.x_ - vector1.x_ * vector2.z_},
  z_ {vector1.x_ * vector2.y_ - vector1.y_ * vector2.x_},
  magnitude_ {magnitude ()}
{
}

float Vector3D::normalize() noexcept {
  const float len {magnitude ()};
  this->x_ /= len;
  this->y_ /= len;
  this->z_ /= len;
  this->magnitude_ = 1.0f;
  return len;
}

Vector3D Vector3D::returnNormalized() const noexcept {
  const float len {magnitude ()};
  float x {this->x_ / len};
  float y {this->y_ / len};
  float z {this->z_ / len};
  return Vector3D {x, y, z};
}

bool Vector3D::isNull () noexcept {
  return this->x_ == 0 && this->y_ == 0 && this->z_ == 0;
}

float Vector3D::dotProduct (const Vector3D vector) const noexcept {
  return this -> x_ * vector . x_ + this -> y_ * vector . y_ + this -> z_ * vector . z_;
}

float Vector3D::dotProduct (const Point3D dest, const Point3D orig) const noexcept {
  return this->x_ * (dest.x_ - orig.x_) +
         this->y_ * (dest.y_ - orig.y_) +
         this->z_ * (dest.z_ - orig.z_);
}

float Vector3D::squareMagnitude() const noexcept {
  return this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_;
}

Vector3D Vector3D::crossProduct (const Vector3D vector) const noexcept {
  const float x {this->y_ * vector.z_ - this->z_ * vector.y_};
  const float y {this->z_ * vector.x_ - this->x_ * vector.z_};
  const float z {this->x_ * vector.y_ - this->y_ * vector.x_};
  const float length {
    std::sqrt (this -> x_ * this -> x_ + this -> y_ * this -> y_ + this -> z_ * this -> z_)};

  return Vector3D {x / length, y / length, z / length};
}

void Vector3D::operator*=(const float value) noexcept {
    this->x_ *= value;
    this->y_ *= value;
    this->z_ *= value;
}

Vector3D Vector3D::operator*(const float value) const noexcept {
  return Vector3D {this -> x_ * value, this -> y_ * value, this -> z_ * value};
}

Point3D Vector3D::operator/(const float value) const noexcept {
  return Point3D {
            this->x_ / value,
            this->y_ / value,
            this->z_ / value};
}

Vector3D Vector3D::operator+ (const Vector3D vector) const noexcept {
  return Vector3D {this->x_ + vector.x_, this->y_ + vector.y_, this->z_ + vector.z_};
}

Vector3D Vector3D::operator- (const Vector3D vector) const noexcept {
  return Vector3D {this->x_ - vector.x_, this->y_ - vector.y_, this->z_ - vector.z_};
}

Vector3D Vector3D::operator* (const Vector3D vector) const noexcept {
  return Vector3D {this->x_ * vector.x_, this->y_ * vector.y_, this->z_ * vector.z_};
}

Vector3D Vector3D::operator-() const noexcept {
  return Vector3D {-this->x_, -this->y_, -this->z_, this->magnitude_};
}

unsigned Vector3D::getInstances () noexcept {
  const unsigned res {counter};
  counter = 0;
  return res;
}
