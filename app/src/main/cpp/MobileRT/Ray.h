//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAY_H
#define MOBILERAYTRACER_RAY_H

#include "myPoint.h"
#include "myVect.h"

namespace MobileRT {
    class Ray {
    private:
    public:
     myPoint* orig;
     myVect* dir;
     float max_T;
     int depth;

        Ray ();

        Ray (myPoint* porig, myVect* pdir);
        Ray (myPoint* porig, myVect* pdir, float pmax_T);
        Ray (myPoint* porig, myVect* pdir, float pmax_T, int d);
    };
}


#endif //MOBILERAYTRACER_RAY_H
