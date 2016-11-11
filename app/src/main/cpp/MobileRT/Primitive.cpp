//
// Created by puscas on 16-10-2016.
//

#include "Primitive.hpp"

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

Intersection* Primitive::Intersect(const Ray& r, float dist)
{
    //Intersection isect (this->shape_->Intersect(r));
    //Intersection isect (this->shape_->Intersect(r, &this->material_));
    /*if (isect.intersected() == true)
    {
        isect.material(&this->material_);
    }*/
    return this->shape_->Intersect(r, &this->material_, dist);
}