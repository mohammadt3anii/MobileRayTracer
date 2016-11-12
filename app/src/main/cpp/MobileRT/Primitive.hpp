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
            const Shape* shape_;
            const Material material_;

        public:
            Primitive (const Shape* shape, const Material& material);
            virtual ~Primitive ();

            bool Intersect(const Ray& ray, Intersection& intersection) const;
    };
}

#endif //MOBILERAYTRACER_PRIMITIVE_HPP