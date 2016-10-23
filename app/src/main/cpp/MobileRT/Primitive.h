//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PRIMITIVE_H
#define MOBILERAYTRACER_PRIMITIVE_H

#include "myShape.h"
#include "myMaterial.h"
#include "Ray.h"

namespace MobileRT {
    class Primitive {
    private:

    public:
        myShape *shape;
        myMaterial *material;
        Primitive (myShape* ps, myMaterial* pm);
        Intersection* Intersect (const Ray& r);
    };
}

#endif //MOBILERAYTRACER_PRIMITIVE_H