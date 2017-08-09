//
// Created by Tiago on 22-Jun-17.
//

#include "Orthographic.hpp"

using Components::Orthographic;
using MobileRT::Point3D;
using MobileRT::Vector3D;
using MobileRT::Ray;

Orthographic::Orthographic (Point3D position, Point3D lookAt, Vector3D up,
                            const float sizeH, const float sizeV) noexcept :
  Camera {position, lookAt, up},
  sizeH_ {sizeH / 2.0f},
  sizeV_ {sizeV / 2.0f} {
}

/* u = x / width */
/* v = y / height */
/* deviationU = [-0.5f / width, 0.5f / width] */
/* deviationV = [-0.5f / height, 0.5f / height] */
Ray Orthographic::generateRay(const float u, const float v,
                              const float deviationU, const float deviationV) const noexcept {

  return Ray {this -> direction_,
              this -> position_ +
             this->right_ * (u - 0.5f) * this->sizeH_ + this->right_ * deviationU +
              this -> up_ * (0.5f - v) * this -> sizeV_ + this -> up_ * deviationV, 1};
}
