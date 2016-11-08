//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "Shape.h"

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
                    Quadratic_Sol () : has_sol (false), t0 (0.0f), t1 (0.0f) {}
                    Quadratic_Sol (const float pt0, const float pt1) : has_sol (true), t0 (pt0), t1 (pt1) {}
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

            virtual Intersection Intersect(const Ray& ray) const;
    };
}

#endif //MOBILERAYTRACER_SPHERE_H