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
            const Point orig;
            Vect dir;
            const float max_T;
            const unsigned int depth;

            Ray ();
            Ray (const Point& porig, const Vect& pdir);
            Ray (const Point& porig, const Vect& pdir, const float& pmax_T);
            Ray (const Point& porig, const Vect& pdir, const float& pmax_T, const unsigned int& d);
    };
}

#endif //MOBILERAYTRACER_RAY_H