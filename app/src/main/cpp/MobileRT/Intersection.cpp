//
// Created by puscas on 16-10-2016.
//

#include "Intersection.hpp"
#include "Constants.hpp"

using namespace MobileRT;

// constructor for no intersection
Intersection::Intersection () :
    intersected_(false),
    length_(MAX_LENGTH),
    material_(nullptr)
{
}

Intersection::Intersection (const Point& point, const Vect& normal, const float length) :
    intersected_(true),
    point_(point),
    normal_(normal),
    length_(length),
    material_(nullptr)
{
}

void Intersection::material(const Material* const material)
{
    this->material_ = material;
}


bool Intersection::intersected() const
{
    return this->intersected_;
}

const Point& Intersection::point() const
{
    return this->point_;
}

const Vect& Intersection::normal() const
{
    return this->normal_;
}

float Intersection::length() const
{
    return this->length_;
}


const Material& Intersection::material() const
{
    return *this->material_;
}