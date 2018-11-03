//
// Created by Tiago on 15-11-2016.
//

#ifndef MOBILERT_SHAPES_TRIANGLE_HPP
#define MOBILERT_SHAPES_TRIANGLE_HPP

#include "MobileRT/Boxes/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Ray.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Triangle final {
    public:
        ::glm::vec3 AC_ {};
        ::glm::vec3 AB_ {};
        ::glm::vec3 pointA_ {};
        ::glm::vec3 normalA_ {};
        ::glm::vec3 normalB_ {};
        ::glm::vec3 normalC_ {};
        ::glm::vec3 colorA_ {};
        ::glm::vec3 colorB_ {};
        ::glm::vec3 colorC_ {};
        ::std::int32_t materialId_ {};

    public:
        explicit Triangle () noexcept = delete;

        explicit Triangle(
                const ::glm::vec3 &pointA, const ::glm::vec3 &pointB,
                const ::glm::vec3 &pointC, ::std::int32_t materialId) noexcept;

        explicit Triangle(
                const ::glm::vec3 &pointA, const ::glm::vec3 &pointB, const ::glm::vec3 &pointC,
                ::std::int32_t materialId,
                const ::glm::vec3 &normalA, const ::glm::vec3 &normalB, const ::glm::vec3 &normalC,
                const ::glm::vec3 &colorA, const ::glm::vec3 &colorB, const ::glm::vec3 &colorC) noexcept;

        explicit Triangle(
                const ::glm::vec3 &pointA, const ::glm::vec3 &pointB, const ::glm::vec3 &pointC,
                ::std::int32_t materialId,
                const ::glm::vec3 &colorA, const ::glm::vec3 &colorB, const ::glm::vec3 &colorC) noexcept;

        Triangle(const Triangle &triangle) noexcept = default;

        Triangle(Triangle &&triangle) noexcept = default;

        ~Triangle() noexcept = default;

        Triangle &operator=(const Triangle &triangle) noexcept = default;

        Triangle &operator=(Triangle &&triangle) noexcept = default;

        bool intersect(Intersection *intersection, const Ray &ray) const noexcept;

        bool intersect(const Ray &ray, float dist) const noexcept;

        void moveTo(float x, float y) noexcept;

        float getZ() const noexcept;

        AABB getAABB() const noexcept;

        bool intersectBox(const AABB &box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_TRIANGLE_HPP
