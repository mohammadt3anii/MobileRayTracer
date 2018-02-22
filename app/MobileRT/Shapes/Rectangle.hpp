//
// Created by Tiago on 10-Jul-17.
//

#ifndef MOBILERT_SHAPES_RECTANGLE_HPP
#define MOBILERT_SHAPES_RECTANGLE_HPP

#include "MobileRT/Accelerators/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Point3D.hpp"
#include "MobileRT/Ray.hpp"
#include "MobileRT/Vector3D.hpp"

namespace MobileRT {
    class Rectangle final {
    private:
        Point3D pointA_{};
        Point3D pointB_{};
        Point3D pointC_{};
        Point3D pointD_{};
        Vector3D AC_{};
        Vector3D AB_{};
        Vector3D BD_{};
        Vector3D CD_{};
        Vector3D normal_{};

    private:
        Point3D getPositionMin() const noexcept;

        Point3D getPositionMax() const noexcept;

    public:
        explicit Rectangle(Point3D pointA, Point3D pointB, Point3D pointC, Point3D pointD) noexcept;

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
