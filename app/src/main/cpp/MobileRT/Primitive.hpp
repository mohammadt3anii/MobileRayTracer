//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PRIMITIVE_HPP
#define MOBILERAYTRACER_PRIMITIVE_HPP

#include "Shape.hpp"
#include "Material.hpp"
#include "Ray.hpp"

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

            Intersection* Intersect(const Ray& ray, float dist);
    };
}

#endif //MOBILERAYTRACER_PRIMITIVE_HPP