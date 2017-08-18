//
// Created by Tiago on 16-10-2016.
//

#include "DepthMap.hpp"

using Components::DepthMap;
using MobileRT::Point3D;
using MobileRT::RGB;
using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::Scene;

DepthMap::DepthMap (Scene &&scene, const Point3D maxPoint, const Accelerator accelerator) noexcept
  :
  Shader {std::move (scene), 0, accelerator},
  maxPoint_ {maxPoint}
{
}

bool DepthMap::shade (RGB *const rgb, const Intersection intersection, Ray &&ray) const noexcept {
  const float maxDist {(maxPoint_ - ray.origin_).normalize () * 1.1f};
  const float depth {intersection.length_};
  *rgb += (maxDist - depth) / maxDist;
  return false;
}

void DepthMap::resetSampling() noexcept {
    this->scene_.resetSampling();
}
