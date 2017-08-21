//
// Created by Tiago on 16-10-2016.
//

#include "Point3D.hpp"

using MobileRT::Point3D;
using MobileRT::Vector3D;
static unsigned counter {0};

Point3D::Point3D(const float x, const float y, const float z) noexcept :
  x_ {x},
  y_ {y},
  z_ {z} {
    counter++;
}

Point3D::Point3D (const Point3D point, const Vector3D vector, const float value) noexcept :
  x_ {vector.x_ * value + point.x_},
  y_ {vector.y_ * value + point.y_},
  z_ {vector.z_ * value + point.z_} {
}

Vector3D Point3D::operator- (const Point3D point) const noexcept {
  return Vector3D {this->x_ - point.x_,
                   this->y_ - point.y_,
                   this->z_ - point.z_};
}

Point3D Point3D::operator+ (const Vector3D vector) const noexcept {
  return Point3D {this->x_ + vector.x_,
                  this->y_ + vector.y_,
                  this->z_ + vector.z_};
}

Point3D Point3D::operator- (float value) const noexcept {
  return Point3D {this->x_ - value,
                  this->y_ - value,
                  this->z_ - value};
}

Point3D Point3D::operator+ (float value) const noexcept {
  return Point3D {this->x_ + value,
                  this->y_ + value,
                  this->z_ + value};
}

unsigned Point3D::getInstances () noexcept {
  const unsigned res {counter};
  counter = 0;
  return res;
}
