//
// Created by puscas on 16-10-2016.
//

#include "Ray.h"

using namespace MobileRT;

Ray::Ray () {
    orig = new myPoint (0.f,0.f,0.f);
    dir = new myVect (0.f,0.f,1.f);
    max_T = 1E10f;
    depth = 0;
}

Ray::Ray (myPoint* porig, myVect* pdir) {
    orig = porig;
    dir = pdir;
    max_T = 1E10f;
    depth = 0;
    dir->normalize();
}
Ray::Ray (myPoint* porig, myVect* pdir, float pmax_T) {
    orig = porig;
    dir = pdir;
    max_T = pmax_T;
    depth = 0;
    dir->normalize();
}
Ray::Ray (myPoint* porig, myVect* pdir, float pmax_T, int d) {
    orig = porig;
    dir = pdir;
    max_T = pmax_T;
    depth = d;
    dir->normalize();
}