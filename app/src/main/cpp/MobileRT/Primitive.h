//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PRIMITIVE_H
#define MOBILERAYTRACER_PRIMITIVE_H

#include "Shape.h"
#include "Material.h"
#include "Ray.h"

namespace MobileRT
{
    class Primitive
    {
        private:
            const Shape* const shape_;
            const Material material_;

        public:
            Primitive (const Shape* const ps, const Material& pm);
            virtual ~Primitive ();

            Intersection Intersect(const Ray& ray);
    };
}

#endif //MOBILERAYTRACER_PRIMITIVE_H