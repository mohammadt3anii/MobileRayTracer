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
    length_(length),
    material_(nullptr),
    point_(point),
    normal_(normal)
{
}

void Intersection::material(const Material& material)
{
    material_ = &material;
}

const Material* Intersection::material() const
{
    return material_;
}

const bool Intersection::intersected() const
{
    return intersected_;
}

const Point Intersection::point() const
{
    return point_;
}

const Vect Intersection::normal() const
{
    return normal_;
}

const float Intersection::length() const
{
    return length_;
}