//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYVEC_H
#define MOBILERAYTRACER_MYVEC_H

#include "myPoint.h"

namespace MobileRT {
    class myVect {
    private:

    public:
        float x,y,z;

        myVect ();
        myVect (const float px, const float py, const float pz);
        myVect (const myVect& v);
        float normalize ();

        // symetric vector
     myVect* symmetric ();

        // dot product
     float dot (const myVect& v);
     void mult (const float f);
     void sub (const myVect& v);
     float length ();

     void add (const myVect& v);

     //dot product
     float dot (const myPoint& v);
    };
}

#endif //MOBILERAYTRACER_MYVEC_H