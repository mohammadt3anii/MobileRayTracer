//
// Created by puscas on 31/07/17.
//

#include "DiffuseMaterial.hpp"

using ::Components::DiffuseMaterial;
using ::MobileRT::Intersection;
using ::MobileRT::RGB;
using ::MobileRT::Ray;
using ::MobileRT::Scene;

DiffuseMaterial::DiffuseMaterial(Scene &&scene, const Accelerator accelerator) noexcept :
        Shader{::std::move(scene), 0, accelerator} {
}

bool DiffuseMaterial::shade(RGB *const rgb, const Intersection intersection, Ray && /*ray*/) noexcept {
    *rgb = intersection.material_->Kd_;
    return false;
}

void DiffuseMaterial::resetSampling() noexcept {
    this->scene_.resetSampling();
}
