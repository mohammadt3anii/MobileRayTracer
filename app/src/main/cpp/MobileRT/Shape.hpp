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
        private:
            

        public:
            Shape ();
            virtual ~Shape ();

            //virtual Intersection Intersect(const Ray&) const;
            virtual Intersection* Intersect(const Ray&, const Material* material, const float maxRayDist) const;
    };
}

#endif //MOBILERAYTRACER_MYSHAPE_HPP