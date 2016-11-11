//
// Created by puscas on 16-10-2016.
//

#include "Shape.hpp"

using namespace MobileRT;

Shape::Shape ()
{
}

Shape::~Shape ()
{
}

Intersection* Shape::Intersect(const Ray&, const Material*, float) const
{
    return nullptr;
}