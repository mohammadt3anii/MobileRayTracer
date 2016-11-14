//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_VEC_H
#define MOBILERAYTRACER_VEC_H

#include "Point.h"

namespace MobileRT
{
    class Vect
    {
        private:
            

        public:
            float x_;
            float y_;
            float z_;

            Vect ();
            Vect (const float px, const float py, const float pz);
            Vect (const Vect& v);

            float normalize ();
            Vect returnNormalized () const;

            // symetric vector
            Vect symmetric () const;

            // dot product
            float dot (const Vect& v) const;

            float dot() const;
            void mult (const float f);
            void sub (const Vect& v);
            float length ();

            Vect operator*(const float value) const;

            void setVect (const float px, const float py, const float pz);
            void setVect (const Point& dest, const Point& orig);
    };
}

#endif //MOBILERAYTRACER_VEC_H