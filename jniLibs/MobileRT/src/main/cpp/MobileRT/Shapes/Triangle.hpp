//
// Created by Tiago on 15-11-2016.
//

#ifndef MOBILERT_SHAPES_TRIANGLE_HPP
#define MOBILERT_SHAPES_TRIANGLE_HPP

#include "Shape.hpp"
#include "../Point3D.hpp"
#include "../Vector3D.hpp"

namespace MobileRT {
    class Triangle : public Shape {
    public:
        const Vector3D AC_;
        const Vector3D AB_;
        const Vector3D normal_;
        const Point3D pointA_;
        const Point3D pointB_;
        const Point3D pointC_;

    public:
        explicit Triangle(const Point3D &pointA, const Point3D &pointB,
                          const Point3D &pointC) noexcept;

		Triangle(const Triangle &triangle) noexcept = delete;

		Triangle(Triangle &&triangle) noexcept = delete;

        //~Triangle() noexcept override = default;

        Triangle &operator=(const Triangle &triangle) noexcept = delete;

		Triangle &operator=(Triangle &&triangle) noexcept = delete;

        bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept final;

        void moveTo(float x, float y) noexcept final;

        float getZ() const noexcept final;

        Point3D getPositionMin() const noexcept final;

        Point3D getPositionMax() const noexcept final;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_TRIANGLE_HPP
