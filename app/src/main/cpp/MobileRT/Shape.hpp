//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYSHAPE_HPP
#define MOBILERAYTRACER_MYSHAPE_HPP

#include "Ray.hpp"
#include "Intersection.hpp"

namespace MobileRT
{
    class Shape
    {
        public:
            virtual ~Shape() = default;

            virtual bool Intersect(const Ray&, const Material*, const float, Intersection&) = 0;
            virtual bool Intersect(const Ray&, const Material*, Intersection&) = 0;
    };
}

#endif //MOBILERAYTRACER_MYSHAPE_HPP