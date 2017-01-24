//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHAPE_H
#define MOBILERAYTRACER_SHAPE_H

#include "../Intersection.h"
#include "../Ray.h"
#include "../Material.h"

namespace MobileRT
{
    class Shape
    {
    private:
            

        public:
            virtual ~Shape() = default;
            virtual bool intersect(Intersection&, const Ray&, const Material&) const = 0;

        virtual float getZ() const;

        virtual void moveTo(float x, float y);
    };
}

#endif //MOBILERAYTRACER_SHAPE_H