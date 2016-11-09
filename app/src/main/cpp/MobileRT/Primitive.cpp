//
// Created by puscas on 16-10-2016.
//

#include "Primitive.h"

using namespace MobileRT;

Primitive::Primitive (const Shape* const ps, const Material& pm) :
    shape_(ps),
    material_(pm)
{
}

Primitive::~Primitive ()
{
    delete this->shape_;
}

Intersection Primitive::Intersect(const Ray& r)
{
    Intersection isect (this->shape_->Intersect(r));
    if (isect.intersected() == true)
    {
        isect.material(&this->material_);
    }
    return isect;
}