//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Ray.hpp"
#include <atomic>

using ::MobileRT::Ray;

Ray::Ray(const ::glm::vec3 &dir, const ::glm::vec3 &origin,
         const ::std::int32_t depth, const void *const primitive) noexcept :
        origin_{origin},
        direction_{dir},
        depth_{depth},
        primitive_{primitive} {
}
