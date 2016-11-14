//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_INTERSECTION_H
#define MOBILERAYTRACER_INTERSECTION_H

#include "Point.h"
#include "Vect.h"
#include "Material.h"

namespace MobileRT
{
    class Intersection
    {
        private:


    public:
        Point point_;   // intersection point
        Vect normal_;    // intersection normal
        float length_;     // ray length parameter
        const Material *material_;  // material of the intersected primitive
        Vect symNormal_;

        Intersection ();

        void setIntersection (const Point& point, const Vect& normal, const float length, const Material* material);
        Vect& getSymNormal();
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_H