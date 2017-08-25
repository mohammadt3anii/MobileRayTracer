//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::RGB;
using MobileRT::Primitive;
using MobileRT::Shader;

Shader::Shader (Scene &&scene, const unsigned samplesLight, const Accelerator accelerator) noexcept
  :
  scene_ {std::move (scene)},
  accelerator_ {accelerator},
  samplesLight_ {samplesLight}
{
  this->scene_.triangles_.shrink_to_fit ();
  this->scene_.spheres_.shrink_to_fit ();
  this->scene_.planes_.shrink_to_fit ();
  this->scene_.lights_.shrink_to_fit ();
}

void Shader::initializeAccelerators (AABB camera) noexcept {
  Point3D min {RayLengthMax, RayLengthMax, RayLengthMax};
  Point3D max {-RayLengthMax, -RayLengthMax, -RayLengthMax};
  Scene::getBounds<Primitive<Triangle>> (this->scene_.triangles_, &min, &max);
  Scene::getBounds<Primitive<Sphere>> (this->scene_.spheres_, &min, &max);
  Scene::getBounds<Primitive<Plane>> (this->scene_.planes_, &min, &max);
  Scene::getBounds<Primitive<Rectangle>> (this->scene_.rectangles_, &min, &max);
  Scene::AABBbounds (camera, &min, &max);
  AABB sceneBounds {min - Epsilon, max + Epsilon};
  switch (accelerator_) {
    case Accelerator::REGULAR_GRID: {
      regularGrid_ = RegularGrid {sceneBounds, &scene_, 32};
    }
      break;
    case Accelerator::KD_TREE: {
      kDTree_ = KdTree {sceneBounds, &scene_};
    }
      break;

    case Accelerator::NONE:
      break;
  }
}

bool Shader::traceTouch (Intersection *const intersection, Ray &&ray) noexcept {
  return this->scene_.trace(intersection, std::move(ray));
}

Shader::~Shader () noexcept {
	LOG("SHADER DELETED");
}

bool Shader::shadowTrace (Intersection *const intersection, Ray &&ray) noexcept {
  bool intersected {false};
  switch (accelerator_) {
    case Accelerator::REGULAR_GRID:
      intersected = this->regularGrid_.shadowTrace (intersection, std::move (ray));
      break;
    case Accelerator::KD_TREE:
      intersected = this->kDTree_.shadowTrace (intersection, std::move (ray));
      break;
    case Accelerator::NONE:
      intersected = this->scene_.shadowTrace (intersection, std::move (ray));
      break;
  }
  return intersected;
}

bool Shader::rayTrace (RGB *const rgb, Intersection *const intersection, Ray &&ray) noexcept {
  bool intersected {false};
  switch (accelerator_) {
    case Accelerator::REGULAR_GRID:
      intersected = this->regularGrid_.trace (intersection, ray);
      break;
    case Accelerator::KD_TREE:
      intersected = this->kDTree_.trace (intersection, ray);
      break;

    case Accelerator::NONE:
      intersected = this->scene_.trace (intersection, ray);
      break;
  }
  return intersected ? shade (rgb, *intersection, std::move (ray)) : false;
}
