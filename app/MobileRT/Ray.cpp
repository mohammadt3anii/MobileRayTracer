//
// Created by Tiago on 16-10-2016.
//

#include "Ray.hpp"
#include <atomic>

using ::MobileRT::Ray;

namespace {
    int32_t getID() {
        static ::std::atomic<int32_t> id{0};
        const int32_t current{id.fetch_add(1, ::std::memory_order_relaxed)};
        return current;
    }
}//namespace

Ray::Ray(::glm::vec3 dir, ::glm::vec3 origin,
         const int32_t depth, const void *const primitive) noexcept :
        origin_{origin},
        direction_{dir},
        depth_{depth},
        id_{getID()},
        primitive_{primitive} {
}
