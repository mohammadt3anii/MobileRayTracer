//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYSHAPE_H
#define MOBILERAYTRACER_MYSHAPE_H

#include "Ray.h"
#include "Intersection.h"

namespace MobileRT {
    class myShape {
    private:

    public:
        myShape ();

        Intersection* Intersect (Ray r);
    };
}

#endif //MOBILERAYTRACER_MYSHAPE_H