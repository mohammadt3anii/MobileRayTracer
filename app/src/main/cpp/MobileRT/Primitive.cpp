//
// Created by puscas on 16-10-2016.
//

#include "Primitive.hpp"

using namespace MobileRT;

Primitive::Primitive (const Shape* shape, const Material& pm) :
    shape_(shape),
    material_(pm)
{
}

Primitive::~Primitive ()
{
}

bool Primitive::Intersect(const Ray& ray, Intersection& intersection) const
{
    return this->shape_->Intersect(ray, &this->material_, intersection);
}