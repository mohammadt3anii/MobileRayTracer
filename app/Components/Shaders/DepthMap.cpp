//
// Created by Tiago on 16-10-2016.
//

#include "DepthMap.hpp"

using ::Components::DepthMap;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;
using ::MobileRT::Scene;

DepthMap::DepthMap(Scene scene, const glm::vec3 maxPoint, const Accelerator accelerator) noexcept
        :
        Shader{::std::move(scene), 0, accelerator},
        maxPoint_{maxPoint} {
}

bool DepthMap::shade(glm::vec3 *const rgb, const Intersection intersection, Ray ray) noexcept {
    const float maxDist{glm::length(glm::normalize(maxPoint_ - ray.origin_)) * 1.1f};
    const float depth{intersection.length_ >= maxDist ? 0.0f :
                      (maxDist - intersection.length_) / maxDist};
    *rgb = {depth, depth, depth};
    return false;
}

void DepthMap::resetSampling() noexcept {
    this->scene_.resetSampling();
}
