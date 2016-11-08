//
// Created by puscas on 16-10-2016.
//

#include "Primitive.h"

using namespace MobileRT;

Primitive::Primitive (const Shape* const ps, const Material& pm) :
    shape(ps),
    material(pm)
{
}

Intersection Primitive::Intersect(const Ray& r)
{
    Intersection isect (this->shape->Intersect(r));
    if (isect.intersected() == true)
    {
        isect.material(&this->material);
    }
    return isect;
}