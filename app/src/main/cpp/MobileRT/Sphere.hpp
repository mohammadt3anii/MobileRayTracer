//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_HPP
#define MOBILERAYTRACER_SPHERE_HPP

#include "Shape.hpp"

namespace MobileRT
{
    class Sphere : public Shape
    {
        private:
            class Quadratic_Sol
            {
                private:
                    

                public:
                    const bool has_sol;
                    const float t0;
                    const float t1;
                    Quadratic_Sol ();
                    Quadratic_Sol (const float pt0, const float pt1);
            };

            const Point center;
            const Point sq_center;
            const float radius;
            const float sq_radius;
            Quadratic_Sol Quadratic (const float A, const float B, const float C) const;

        public:
            Sphere ();
            Sphere (const float r);
            Sphere (const Point& c);
            Sphere (const Point& c, const float r);

            bool Intersect(const Ray& ray, const Material* material, const float maxRayDist, Intersection& intersection) override;
            bool Intersect(const Ray& ray, const Material* material, Intersection& intersection) override;
    };
}

#endif //MOBILERAYTRACER_SPHERE_HPP