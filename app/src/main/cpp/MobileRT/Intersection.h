//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_INTERSECTION_H
#define MOBILERAYTRACER_INTERSECTION_H

#include "Point.h"
#include "Vect.h"
#include "Material.h"

namespace MobileRT {
    class Intersection {
    private:
        bool intersected_; // was there an intersection?
        Point point_;   // intersection point
        Vect normal_;    // intersection normal
        float length_;     // ray length parameter
        const Material* material_;  // material of the intersected primitive

    public:
        Intersection ();
        Intersection (const Point& point, const Vect& normal, const float& length);

        void material(const Material& material);
        const Material* material() const;

        const bool intersected() const;
        const Point point() const;
        const Vect normal() const;
        const float length() const;
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_H