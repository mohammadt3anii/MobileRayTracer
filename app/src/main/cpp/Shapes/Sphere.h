//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "../../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Shapes/Shape.h"
#include "../../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Point3D.h"

namespace MobileRT {
    class Sphere : public Shape {
    private:
        const float sq_radius_;
        Point3D center_;
        char padding[4] __attribute__((unused));

    public:
        Sphere(const Point3D &center, const float radius);

        bool intersect(Intersection &intersection, const Ray &ray,
                       const Material &material) const override;

        void moveTo(float x, float y) override;

        float getZ() const override;
    };
}

#endif //MOBILERAYTRACER_SPHERE_H
