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

bool Primitive::Intersect(const Ray& r, const float maxRayDist, Intersection& intersection)
{
    //Intersection isect (this->shape_->Intersect(r));
    //Intersection isect (this->shape_->Intersect(r, &this->material_));
    /*if (isect.intersected() == true)
    {
        isect.material(&this->material_);
    }*/
    //return this->shape_->Intersect(r, &this->material_, maxRayDist);
    return this->shape_->Intersect(r, &this->material_, maxRayDist, intersection);
}

bool Primitive::Intersect(const Ray& r, Intersection& intersection)
{
    return this->shape_->Intersect(r, &this->material_, intersection);
}