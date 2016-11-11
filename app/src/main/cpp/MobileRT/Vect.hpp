//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_VEC_HPP
#define MOBILERAYTRACER_VEC_HPP

#include "Point.hpp"

namespace MobileRT
{
    class Vect
    {
        private:
            

        public:
            float x;
            float y;
            float z;
            
            Vect ();
            Vect (const float px, const float py, const float pz);
            Vect (const Vect& v);

            float normalize ();
            Vect returnNormalized () const;

            // symetric vector
            Vect symmetric () const;

            // dot product
            float dot (const Vect& v) const;
            void mult (const float f);
            void sub (const Vect& v);
            float length ();

            void add (const Vect& v);

            //dot product
            float not_dot (const Point& v) const;

            Vect operator*(const float value) const;
    };
}

#endif //MOBILERAYTRACER_VEC_HPP