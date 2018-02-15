//
// Created by puscas on 06-03-2017.
//

#ifndef MOBILERT_ACCELERATORS_AABB_HPP
#define MOBILERT_ACCELERATORS_AABB_HPP

#include "MobileRT/Intersection.hpp"
#include "MobileRT/Material.hpp"
#include "MobileRT/Ray.hpp"
#include <vector>

namespace MobileRT {
    class AABB final {
    public:
        Point3D pointMin_{};
        Point3D pointMax_{};

    public:
        explicit AABB() noexcept = default;

        explicit AABB(Point3D pointMin, Point3D pointMax) noexcept;

        AABB(const AABB &AABB) noexcept = default;

        AABB(AABB &&AABB) noexcept = default;

        ~AABB() noexcept = default;

        AABB &operator=(const AABB &AABB) noexcept = default;

        AABB &operator=(AABB &&AABB) noexcept = default;

        float getSurfaceArea() const noexcept;

        int getLongestAxis() const noexcept;
    };

    AABB surroundingBox(AABB box1, AABB box2) noexcept;

    bool intersect(AABB box, Ray ray) noexcept;
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_AABB_HPP
