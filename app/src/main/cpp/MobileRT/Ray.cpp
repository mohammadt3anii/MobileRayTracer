//
// Created by puscas on 16-10-2016.
//

#include "Ray.h"
#include "Constants.h"

using namespace MobileRT;

Ray::Ray () {
    orig = new Point (0.f,0.f,0.f);
    dir = new Vect (0.f,0.f,1.f);
    max_T = MAX_LENGTH;
    depth = 0;
}

Ray::Ray (Point* porig, Vect* pdir) {
    orig = porig;
    dir = pdir;
    max_T = MAX_LENGTH;
    depth = 0;
    dir->normalize();
}

Ray::Ray (Point* porig, Vect* pdir, float pmax_T) {
    orig = porig;
    dir = pdir;
    max_T = pmax_T;
    depth = 0;
    dir->normalize();
}

Ray::Ray (Point* porig, Vect* pdir, float pmax_T, int d) {
    orig = porig;
    dir = pdir;
    max_T = pmax_T;
    depth = d;
    dir->normalize();
}