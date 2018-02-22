//
// Created by Tiago on 10-Jul-17.
//

#ifndef MOBILERT_SHAPES_RECTANGLE_HPP
#define MOBILERT_SHAPES_RECTANGLE_HPP

#include "MobileRT/Accelerators/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Ray.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Rectangle final {
    private:
        glm::vec3 pointA_{};
        glm::vec3 pointB_{};
        glm::vec3 pointC_{};
        glm::vec3 pointD_{};
        glm::vec3 AC_{};
        glm::vec3 AB_{};
        glm::vec3 BD_{};
        glm::vec3 CD_{};
        glm::vec3 normal_{};

    private:
        glm::vec3 getPositionMin() const noexcept;

        glm::vec3 getPositionMax() const noexcept;

    public:
        explicit Rectangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD) noexcept;

        Rectangle(const Rectangle &rectangle) noexcept = default;

        Rectangle(Rectangle &&rectangle) noexcept = default;

        ~Rectangle() noexcept = default;

        Rectangle &operator=(const Rectangle &rectangle) noexcept = default;

        Rectangle &operator=(Rectangle &&rectangle) noexcept = default;

        Intersection intersect(Intersection intersection, Ray ray) const noexcept;

        void moveTo(float x, float y) noexcept;

        float getZ() const noexcept;

        AABB getAABB() const noexcept;

        bool intersect(AABB box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_RECTANGLE_HPP
