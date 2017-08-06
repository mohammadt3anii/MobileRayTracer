//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::RGB;
using MobileRT::Shader;

Shader::Shader(Scene &&scene, const unsigned int samplesLight) noexcept :
  scene_(std::move(scene)),
  samplesLight_(samplesLight)
{
    this->scene_.triangles_.shrink_to_fit();
		this->scene_.spheres_.shrink_to_fit();
		this->scene_.planes_.shrink_to_fit();
    this->scene_.lights_.shrink_to_fit();

  Point3D min(1000.0f, 1000.0f, 1000.0f);
  Point3D max(- 1000.0f, - 1000.0f, - 1000.0f);
  getSceneBounds<Triangle>(this->scene_.triangles_, &min, &max);
  getSceneBounds<Sphere>(this->scene_.spheres_, &min, &max);
  getSceneBounds<Plane>(this->scene_.planes_, &min, &max);

  const float offset(1.0f);
  min.x_ -= offset;
  min.y_ -= offset;
  min.z_ -= offset;
  max.x_ += offset;
  max.y_ += offset;
  max.z_ += offset;
  regularGrid_ = RegularGrid(min, max, &scene_, 8, 3);
}

template<typename T>
void Shader::getSceneBounds(std::vector<T> primitives, Point3D *min, Point3D *max) {

  for (T &primitive : primitives) {
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

int Shader::traceTouch(Intersection *intersection, Ray &&ray) const noexcept {

  return this->scene_.trace(intersection, std::move(ray));
}

Shader::~Shader () noexcept {
	LOG("SHADER DELETED");
}

bool Shader::rayTrace(RGB *rgb, Intersection *intersection, Ray &&ray) const noexcept {

  if (regularGrid_.intersect(intersection, ray))
    //if (this->scene_.trace(intersection, ray) >= 0)
	{// compute radiance
    intersection->material_ = &this->scene_.materials_[static_cast<uint32_t>(0)];
		return shade(rgb, *intersection, std::move(ray)); 
	}
	return false;
}
