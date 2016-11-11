//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_INTERSECTION_HPP
#define MOBILERAYTRACER_INTERSECTION_HPP

#include "Point.hpp"
#include "Vect.hpp"
#include "Material.hpp"

namespace MobileRT
{
    class Intersection
    {
        private:
            bool intersected_; // was there an intersection?
            Point point_;   // intersection point
            Vect normal_;    // intersection normal
            float length_;     // ray length parameter
            const Material* material_;  // material of the intersected primitive

        public:
            Intersection ();
            Intersection (const Point& point, const Vect& normal, const float length);

            void material(const Material* const material);
            
            bool intersected() const;
            const Point& point() const;
            const Vect& normal() const;
            float length() const;
            const Material& material() const;
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_HPP