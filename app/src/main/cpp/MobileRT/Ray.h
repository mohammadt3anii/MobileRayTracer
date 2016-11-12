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
            float maxDistance;
            const unsigned int depth;

            Ray ();
            Ray (const Point& porig, const Vect& pdir);
            Ray (const Point& porig, const Vect& pdir, const float pmax_T, const unsigned int d);

            void setRay (const float px, const float py, const float pz, const Point& org);
    };
}

#endif //MOBILERAYTRACER_RAY_H