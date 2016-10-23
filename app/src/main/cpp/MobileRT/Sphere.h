//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_H
#define MOBILERAYTRACER_SPHERE_H

#include "myShape.h"

namespace MobileRT {
    class Sphere : public myShape {
    private:
        class Quadratic_Sol {
        private:

        public:
            bool has_sol;
            float t0, t1;
            Quadratic_Sol () { has_sol = false; t0 = 0.f; t1 = 0.f; }
            Quadratic_Sol (float pt0, float pt1) { has_sol = true; t0 = pt0; t1 = pt1; }
        };

        myPoint* center;
        myPoint* sq_center;
        float radius, sq_radius;
        Quadratic_Sol* Quadratic (float A, float B, float C);

    public:
        Sphere ();
        Sphere (float r);
        Sphere (myPoint* c);
        Sphere (myPoint* c, float r);
        void square_params ();
        virtual Intersection* Intersect (const Ray& r);
    };
}

#endif //MOBILERAYTRACER_SPHERE_H