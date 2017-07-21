//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHAPES_SPHERE_HPP
#define MOBILERT_SHAPES_SPHERE_HPP

#include "Shape.hpp"

namespace MobileRT {
    class Sphere final : public Shape {
    private:
        Point3D center_;
        const float sq_radius_;

    public:
        explicit Sphere(const Point3D &center, float radius) noexcept;

		Sphere(const Sphere &sphere) noexcept = delete;

		Sphere(Sphere &&sphere) noexcept = delete;

        //~Sphere() noexcept final = default;

        Sphere &operator=(const Sphere &sphere) noexcept = delete;

		Sphere &operator=(Sphere &&sphere) noexcept = delete;

        bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept final;

        void moveTo(float x, float y) noexcept final;

        float getZ() const noexcept final;

        Point3D getPositionMin() const noexcept final;

        Point3D getPositionMax() const noexcept final;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_SPHERE_HPP
