//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "Shape.h"

namespace MobileRT
{
    class Sphere : public Shape
    {
        private:
            const Point3D center_;
            const float sq_radius_;

        public:
            Sphere (const Point3D& center, const float radius);

            bool intersect(Intersection& intersection, const Ray& ray, const Material* material) const override;
    };
}

#endif //MOBILERAYTRACER_SPHERE_H