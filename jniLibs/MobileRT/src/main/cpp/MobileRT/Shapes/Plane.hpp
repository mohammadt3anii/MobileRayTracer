//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHAPES_PLANE_HPP
#define MOBILERT_SHAPES_PLANE_HPP

#include "Shape.hpp"

namespace MobileRT {
    class Plane final : public Shape {
    private:
        const Point3D point_;   // point in the plane
        const Vector3D normal_;    // normal to the plane

    public:
        explicit Plane(const Point3D &point, const Vector3D &normal) noexcept;

		Plane(const Plane &plane) noexcept = delete;

		Plane(Plane &&plane) noexcept = delete;

		~Plane() noexcept final = default;

        Plane &operator=(const Plane &plane) noexcept = delete;

		Plane &operator=(Plane &&plane) noexcept = delete;

        bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept final;

        void moveTo(float x, float y) noexcept final;

        float getZ() const noexcept final;

        Point3D getPositionMin() const noexcept final;

        Point3D getPositionMax() const noexcept final;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_PLANE_HPP
