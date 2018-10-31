//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Intersection.hpp"

using ::MobileRT::Intersection;

Intersection::Intersection(const float dist) noexcept :
        length_ {dist} {
}

Intersection::Intersection(
        const ::glm::vec3 &intPoint,
        const float dist,
        const ::glm::vec3 &normal,
        const void *const primitive,
        const ::std::int32_t materialId) noexcept :
        point_(intPoint),
        normal_(normal),
        length_(dist),
        primitive_(primitive),
        materialId_(materialId) {
}
