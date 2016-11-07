//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAY_H
#define MOBILERAYTRACER_RAY_H

#include "Point.h"
#include "Vect.h"

namespace MobileRT
{
    class Ray
    {
        private:

        public:
            Point orig;
            Vect dir;
            float max_T;
            int depth;

            Ray ();
            Ray (const Point& porig, const Vect& pdir);
            Ray (const Point& porig, const Vect& pdir, const float& pmax_T);
            Ray (const Point& porig, const Vect& pdir, const float& pmax_T, const int& d);
    };
}

#endif //MOBILERAYTRACER_RAY_H