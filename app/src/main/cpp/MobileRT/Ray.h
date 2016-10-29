//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAY_H
#define MOBILERAYTRACER_RAY_H

#include "Point.h"
#include "Vect.h"

namespace MobileRT {
    class Ray {
    private:

    public:
        Point* orig;
        Vect* dir;
        float max_T;
        int depth;

        Ray ();
        Ray (Point* porig, Vect* pdir);
        Ray (Point* porig, Vect* pdir, float pmax_T);
        Ray (Point* porig, Vect* pdir, float pmax_T, int d);
    };
}

#endif //MOBILERAYTRACER_RAY_H