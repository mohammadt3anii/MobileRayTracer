//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PRIMITIVE_H
#define MOBILERAYTRACER_PRIMITIVE_H

#include "Shapes/Shape.h"
#include "Material.h"
#include "Ray.h"

namespace MobileRT {
    class Primitive {
    private:
        const Shape *shape_;
        const Material material_;

    public:
        Primitive(const Shape *shape, const Material &material);

        ~Primitive();

        bool intersect(Intersection &intersection, const Ray &ray) const;
    };
}

#endif //MOBILERAYTRACER_PRIMITIVE_H