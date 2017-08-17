//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::RGB;
using MobileRT::Shader;

Shader::Shader (Scene &&scene, const unsigned samplesLight, const Accelerator accelerator) noexcept
  :
  scene_ {std::move (scene)},
  accelerator_ {accelerator},
  samplesLight_ {samplesLight}
{
  this->scene_.ptriangles_.shrink_to_fit ();
  this->scene_.pspheres_.shrink_to_fit ();
  this->scene_.pplanes_.shrink_to_fit ();
  this->scene_.lights_.shrink_to_fit();

  Point3D min {1000.0f, 1000.0f, 1000.0f};
  Point3D max {-1000.0f, -1000.0f, -1000.0f};
  getSceneBounds<MobileRT::Primitive<Triangle>> (this->scene_.ptriangles_, &min, &max);
  getSceneBounds<MobileRT::Primitive<Sphere>> (this->scene_.pspheres_, &min, &max);
  getSceneBounds<MobileRT::Primitive<Plane>> (this->scene_.pplanes_, &min, &max);

  const float offset {1.0f};
  min.x_ -= offset;
  min.y_ -= offset;
  min.z_ -= offset;
  max.x_ += offset;
  max.y_ += offset;
  max.z_ += offset;
  regularGrid_ = RegularGrid {min, max, &scene_, 8, 3};
}

template<typename T>
void
Shader::getSceneBounds (const std::vector<T> primitives, Point3D *const min, Point3D *const max) {
  for (const T &primitive : primitives) {
    if (primitive.getAABB().pointMin_.x_ < min->x_) {
      min->x_ = primitive.getAABB().pointMin_.x_;
    }
    if (primitive.getAABB().pointMin_.y_ < min->y_) {
      min->y_ = primitive.getAABB().pointMin_.y_;
    }
    if (primitive.getAABB().pointMin_.z_ < min->z_) {
      min->z_ = primitive.getAABB().pointMin_.z_;
    }
    if (primitive.getAABB().pointMax_.x_ > max->x_) {
      max->x_ = primitive.getAABB().pointMax_.x_;
    }
    if (primitive.getAABB().pointMax_.y_ > max->y_) {
      max->y_ = primitive.getAABB().pointMax_.y_;
    }
    if (primitive.getAABB().pointMax_.z_ > max->z_) {
      max->z_ = primitive.getAABB().pointMax_.z_;
    }
  }
}

int Shader::traceTouch (Intersection *const intersection, Ray &&ray) const noexcept {
  return this->scene_.trace(intersection, std::move(ray));
}

Shader::~Shader () noexcept {
	LOG("SHADER DELETED");
}

bool Shader::rayTrace (RGB *const rgb, Intersection *const intersection, Ray &&ray) const noexcept {
  bool intersected {false};
  switch (accelerator_) {
    case Accelerator::REGULAR_GRID:
      intersected = this->regularGrid_.intersect (intersection, ray);
      break;

    case Accelerator::NONE:
      intersected = (this->scene_.trace (intersection, ray) >= 0);
  }
  return intersected ? shade (rgb, *intersection, std::move (ray)) : false;
}
