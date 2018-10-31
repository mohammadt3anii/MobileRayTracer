//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHAPES_SPHERE_HPP
#define MOBILERT_SHAPES_SPHERE_HPP

#include "MobileRT/Boxes/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Ray.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Sphere final {
    private:
        ::glm::vec3 center_ {};
        float sq_radius_ {};
        ::std::int32_t materialId_ {};

    public:
        explicit Sphere () noexcept = delete;

        explicit Sphere(const ::glm::vec3 &center, float radius, ::std::int32_t materialId) noexcept;

        Sphere(const Sphere &sphere) noexcept = default;

        Sphere(Sphere &&sphere) noexcept = default;

        ~Sphere() noexcept = default;

        Sphere &operator=(const Sphere &sphere) noexcept = default;

        Sphere &operator=(Sphere &&sphere) noexcept = default;

        bool intersect(Intersection *intersection, const Ray &ray) const noexcept;

        bool intersect(const Ray &ray, float dist) const noexcept;

        void moveTo(float x, float y) noexcept;

        float getZ() const noexcept;

        AABB getAABB() const noexcept;

        bool intersectBox(const AABB &box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_SPHERE_HPP
