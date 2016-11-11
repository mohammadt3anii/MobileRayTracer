//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAY_HPP
#define MOBILERAYTRACER_RAY_HPP

#include "Point.hpp"
#include "Vect.hpp"

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

            Ray (const Point& porig, const Vect& pdir);
            Ray (const Point& porig, const Vect& pdir, const float pmax_T, const unsigned int d);
    };
}

#endif //MOBILERAYTRACER_RAY_HPP