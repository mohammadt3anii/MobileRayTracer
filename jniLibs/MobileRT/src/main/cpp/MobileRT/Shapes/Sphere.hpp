//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "Shape.hpp"

namespace MobileRT {
    class Sphere final : public Shape {
    private:
        const float sq_radius_;
        Point3D center_;

    public:
        explicit Sphere(const Point3D &center, float radius) noexcept;

        bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept final;

        void moveTo(float x, float y) noexcept final;

        float getZ() const noexcept final;

        const Point3D getPositionMin() const noexcept final;

        const Point3D getPositionMax() const noexcept final;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_SPHERE_H
