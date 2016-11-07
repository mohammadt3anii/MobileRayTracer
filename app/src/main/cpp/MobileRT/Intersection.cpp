//
// Created by puscas on 16-10-2016.
//

#include "Intersection.h"
#include "Constants.h"

using namespace MobileRT;

// constructor for no intersection
Intersection::Intersection () :
    intersected_(false),
    length_(MAX_LENGTH),
    material_(nullptr)
{
}

Intersection::Intersection (const Point& point, const Vect& normal, const float& length) :
    intersected_(true),
    point_(point),
    normal_(normal),
    length_(length),
    material_(nullptr)
{
}

void Intersection::material(Material& material)
{
    this->material_ = &material;
}

Material* Intersection::material() const
{
    return this->material_;
}

bool Intersection::intersected() const
{
    return this->intersected_;
}

Point Intersection::point() const
{
    return this->point_;
}

Vect Intersection::normal() const
{
    return this->normal_;
}

float Intersection::length() const
{
    return this->length_;
}