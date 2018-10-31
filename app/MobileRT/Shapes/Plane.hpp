//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHAPES_PLANE_HPP
#define MOBILERT_SHAPES_PLANE_HPP

#include "MobileRT/Boxes/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Ray.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Plane final {
    private:
        ::glm::vec3 normal_{};    // normal to the plane
        ::glm::vec3 point_{};   // point in the plane
        ::std::int32_t materialId_ {};

    private:
        ::glm::vec3 getRightVector() const noexcept;

    public:
        explicit Plane () noexcept = delete;

        explicit Plane(const ::glm::vec3 &point, const ::glm::vec3 &normal,
            ::std::int32_t materialId) noexcept;

        Plane(const Plane &plane) noexcept = default;

        Plane(Plane &&plane) noexcept = default;

        ~Plane() noexcept = default;

        Plane &operator=(const Plane &plane) noexcept = default;

        Plane &operator=(Plane &&plane) noexcept = default;

        bool intersect(Intersection *intersection, const Ray &ray) const noexcept;

        bool intersect(const Ray &ray, float dist) const noexcept;

        void moveTo(float x, float y) noexcept;

        float getZ() const noexcept;

        AABB getAABB() const noexcept;

        float distance(const ::glm::vec3 &point) const noexcept;

        bool intersectBox(const AABB &box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_PLANE_HPP
