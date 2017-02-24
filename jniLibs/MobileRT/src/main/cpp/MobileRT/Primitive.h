//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PRIMITIVE_H
#define MOBILERAYTRACER_PRIMITIVE_H

#include "Shapes/Shape.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Shapes/Triangle.h"
#include "Shapes/Sphere.h"
#include "Shapes/Plane.h"

namespace MobileRT {
    class Primitive {
    private:
        const Material material_;

    public:
        Shape *const shape_;
        Plane *const plane_;
        Sphere *const sphere_;
        Triangle *const triangle_;
        std::function<bool(Intersection &, const Ray &)> const intersect;

    public:
        explicit Primitive(Shape &shape, const Material &material);

        explicit Primitive(Plane &plane, const Material &material);

        explicit Primitive(Sphere &sphere, const Material &material);

        explicit Primitive(Triangle &triangle, const Material &material);

        ~Primitive();
        //bool intersect(Intersection &intersection, const Ray &ray) const;

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_PRIMITIVE_H
