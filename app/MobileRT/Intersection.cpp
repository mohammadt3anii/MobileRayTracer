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
        const ::glm::vec3 &sphereCenter) noexcept :
        point_(intPoint),
        normal_(::glm::normalize(intPoint - sphereCenter)),
        symNormal_(-::glm::normalize(intPoint - sphereCenter)),
        length_(dist) {
}

Intersection::Intersection(
        const ::glm::vec3 &orig,
        const ::glm::vec3 &dir,
        const float dist,
        const ::glm::vec3 &normal,
        const void *const primitive) noexcept :
        point_(orig + dir * dist),
        normal_(normal),
        symNormal_(-normal),
        length_(dist),
        primitive_(primitive) {
}
