//
// Created by puscas on 06-03-2017.
//

#ifndef MOBILERT_ACCELERATORS_AABB_HPP
#define MOBILERT_ACCELERATORS_AABB_HPP

#include "MobileRT/Intersection.hpp"
#include "MobileRT/Material.hpp"
#include "MobileRT/Ray.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace MobileRT {
    class AABB final {
    public:
        ::glm::vec3 pointMin_{};
        ::glm::vec3 pointMax_{};

    public:
        explicit AABB() noexcept = default;

        explicit AABB(::glm::vec3 pointMin, ::glm::vec3 pointMax) noexcept;

        AABB(const AABB &AABB) noexcept = default;

        AABB(AABB &&AABB) noexcept = default;

        ~AABB() noexcept = default;

        AABB &operator=(const AABB &AABB) noexcept = default;

        AABB &operator=(AABB &&AABB) noexcept = default;

        float getSurfaceArea() const noexcept;

        ::std::int32_t getLongestAxis() const noexcept;
    };

    AABB surroundingBox(AABB box1, AABB box2) noexcept;

    bool intersect(AABB box, Ray ray) noexcept;
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_AABB_HPP
