//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PLANE_HPP
#define MOBILERAYTRACER_PLANE_HPP

#include "Shape.hpp"

namespace MobileRT
{
    class Plane : public Shape
    {
        private:
            const Point point_;   // point in the plane
            const Vect normal_;    // normal to the plane
            //const float d_;    // 0th degree coefiicient of the plane equation

        public:
            Plane (const Point& point, const Vect& normal);
            bool Intersect(const Ray& ray, const Material* material, Intersection& intersection) const override;
    };
}

#endif //MOBILERAYTRACER_PLANE_HPP