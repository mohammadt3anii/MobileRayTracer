//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYVEC_H
#define MOBILERAYTRACER_MYVEC_H

#include "Point.h"

namespace MobileRT {
    class Vect {
    private:

    public:
        float x,y,z;

        Vect ();
        Vect (const float& px, const float& py, const float& pz);
        Vect (const Vect& v);
        float normalize ();

        // symetric vector
        Vect symmetric () const;

        // dot product
        float dot (const Vect& v);
        void mult (const float& f);
        void sub (const Vect& v);
        float length ();

        void add (const Vect& v);

        //dot product
        float not_dot (const Point& v);



        Vect operator*(const float& value);
    };
}

#endif //MOBILERAYTRACER_MYVEC_H