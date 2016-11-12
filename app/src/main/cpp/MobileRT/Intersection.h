//
// Created by puscas on 16-10-2016.
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
            bool intersected_; // was there an intersection?
            Point point_;   // intersection point
            Vect normal_;    // intersection normal
            float length_;     // ray length parameter
            const Material* material;  // material of the intersected primitive

            Intersection ();
            Intersection (const Intersection& intersection);
            Intersection (const Point& point, const Vect& normal, const float length, const Material* material);

            //void material (const Material* const material);
            
            bool intersected () const;
            const Point& point () const;
            const Vect& normal () const;
            float length () const;
            void setIntersection (const Intersection& intersection);
            void setIntersection (const Point& point, const Vect& normal, const float length, const Material* material);
            //const Material& material () const;
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_H