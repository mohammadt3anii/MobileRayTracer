//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PLANE_H
#define MOBILERAYTRACER_PLANE_H

#include "Shape.h"

namespace MobileRT {
    class Plane : public Shape {
    private:
        const Point3D point_;   // point in the plane
        const Vector3D normal_;    // normal to the plane

    public:
        explicit Plane(const Point3D &point, const Vector3D &normal);

        bool intersect(Intersection &intersection, const Ray &ray,
                       const Material &material) override;
    };
}

#endif //MOBILERAYTRACER_PLANE_H
