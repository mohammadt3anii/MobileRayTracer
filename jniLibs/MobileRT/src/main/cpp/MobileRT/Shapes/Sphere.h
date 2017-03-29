//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "Shape.h"
#include "../Point3D.h"

namespace MobileRT {
    class Sphere : public Shape {
    private:
        const float sq_radius_;
        Point3D center_;

    public:
        explicit Sphere(const Point3D &center, const float radius);

        bool intersect(Intersection &intersection, const Ray &ray,
                       const Material &material) const override;

        void moveTo(float x, float y) override;

        float getZ(void) const override;
    };
}

#endif //MOBILERAYTRACER_SPHERE_H
