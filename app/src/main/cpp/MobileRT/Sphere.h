//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "Shape.h"

namespace MobileRT {
    class Sphere : public Shape {
    private:
        class Quadratic_Sol
        {
            private:

            public:
                bool has_sol;
                float t0, t1;
                Quadratic_Sol () { has_sol = false; t0 = 0.0f; t1 = 0.0f; }
                Quadratic_Sol (float pt0, float pt1) { has_sol = true; t0 = pt0; t1 = pt1; }
        };

        Point* center;
        Point* sq_center;
        float radius, sq_radius;
        Quadratic_Sol Quadratic (float A, float B, float C);

    public:
        Sphere ();
        Sphere (float r);
        Sphere (Point* c);
        Sphere (Point* c, float r);
        void square_params ();

        virtual Intersection Intersect(const Ray &ray);
    };
}

#endif //MOBILERAYTRACER_SPHERE_H