//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Intersection.hpp"

using ::MobileRT::Intersection;

Intersection::Intersection(float dist, const void *const primitive) noexcept :
        length_ {dist},
        primitive_{primitive} {
}

Intersection::Intersection(
    const ::glm::vec3 &intPoint,
    const float dist,
    const ::glm::vec3 &sphereCenter) noexcept {
        this->point_ = intPoint;
        this->normal_ = ::glm::normalize(this->point_ - sphereCenter);
        this->symNormal_ = -normal_;
        this->length_ = dist;
}

Intersection::Intersection(
    const ::glm::vec3 &orig,
    const ::glm::vec3 &dir,
    const float dist,
    const ::glm::vec3 &normal,
    const void *const primitive) noexcept {
        this->point_ = orig + dir * dist;
        this->normal_ = normal;
        this->symNormal_ = -normal;
        this->length_ = dist;
        this->primitive_ = primitive;
}
