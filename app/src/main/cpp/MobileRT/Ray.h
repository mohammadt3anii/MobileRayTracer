//
// Created by Tiago on 16-10-2016.
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
        Point origin_;
        Vect direction_;
        float maxDistance_;
        const unsigned int depth_;

            Ray ();

        Ray(const Point &orig, const Vect &dir, const float maxDist, const unsigned int depth);

            void setRay (const float px, const float py, const float pz, const Point& org);
    };
}

#endif //MOBILERAYTRACER_RAY_H