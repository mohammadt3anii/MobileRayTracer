//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYSHAPE_H
#define MOBILERAYTRACER_MYSHAPE_H

#include "Ray.h"
#include "Intersection.h"

namespace MobileRT
{
    class Shape
    {
    private:
            

        public:
            virtual ~Shape() = default;
            virtual bool Intersect(const Ray&, const Material*, Intersection&) const = 0;
    };
}

#endif //MOBILERAYTRACER_MYSHAPE_H