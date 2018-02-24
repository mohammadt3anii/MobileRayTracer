//
// Created by Tiago on 16-10-2016.
//

#include "Intersection.hpp"

using ::MobileRT::Intersection;

Intersection::Intersection(const void *primitive) noexcept :
        primitive_{primitive} {
}

Intersection::Intersection(const ::glm::vec3 intPoint,
                         const float dist, const ::glm::vec3 center) noexcept {
    this->point_ = intPoint;
    this->normal_ = ::glm::normalize(this->point_ - center);
    this->symNormal_ = -normal_;
    this->length_ = dist;
}

Intersection::Intersection(const ::glm::vec3 orig,
                         const ::glm::vec3 dir,
                         const float dist,
                         const ::glm::vec3 normal,
                         const void *const primitive) noexcept {
    this->point_ = orig + dir * dist;
    this->normal_ = normal;
    this->symNormal_ = -normal;
    this->length_ = dist;
    this->primitive_ = primitive;
}
