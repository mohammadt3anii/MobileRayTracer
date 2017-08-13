//
// Created by puscas on 31/07/17.
//

#include "DiffuseMaterial.hpp"

using Components::DiffuseMaterial;
using MobileRT::Intersection;
using MobileRT::RGB;
using MobileRT::Ray;
using MobileRT::Scene;

DiffuseMaterial::DiffuseMaterial(Scene &&scene) noexcept :
  Shader {std::move (scene)} {
}

bool DiffuseMaterial::shade (RGB *const rgb, const Intersection intersection,
                             Ray && /*ray*/) const noexcept {
  if (intersection.material_ != nullptr) {
    *rgb += intersection.material_->Kd_;
  }
  return false;
}

void DiffuseMaterial::resetSampling() noexcept {
  this->scene_.resetSampling();
}
