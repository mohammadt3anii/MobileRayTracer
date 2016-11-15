//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHAPE_H
#define MOBILERAYTRACER_SHAPE_H

#include "../Ray.h"
#include "../Intersection.h"

namespace MobileRT
{
    class Shape
    {
    private:
            

        public:
            virtual ~Shape() = default;
            virtual bool intersect(Intersection&, const Ray&, const Material&) const = 0;
    };
}

#endif //MOBILERAYTRACER_SHAPE_H