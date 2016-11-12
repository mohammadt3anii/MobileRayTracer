//
// Created by puscas on 16-10-2016.
//

#include "Primitive.hpp"

using namespace MobileRT;

Primitive::Primitive (Shape* ps, const Material& pm) :
    shape_(ps),
    material_(pm)
{
}

Primitive::~Primitive ()
{
    delete this->shape_;
}

bool Primitive::Intersect(const Ray& r, Intersection& intersection)
{
    return this->shape_->Intersect(r, &this->material_, intersection);
}