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
                    bool has_sol;
                    float t0, t1;
                    Quadratic_Sol () { this->has_sol = false; this->t0 = 0.0f; this->t1 = 0.0f; }
                    Quadratic_Sol (const float pt0, const float pt1) { has_sol = true; t0 = pt0; t1 = pt1; }
            };

            Point* center;
            Point* sq_center;
            float radius;
            float sq_radius;
            Quadratic_Sol Quadratic (const float A, const float B, const float C) const;

        public:
            Sphere ();
            Sphere (const float r);
            Sphere (Point* c);
            Sphere (Point* c, const float r);
            void square_params ();

            virtual Intersection Intersect(const Ray &ray) const;
    };
}

#endif //MOBILERAYTRACER_SPHERE_H