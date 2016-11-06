//
// Created by puscas on 16-10-2016.
//

#include "Primitive.h"
#include <memory>

using namespace MobileRT;

Primitive::Primitive (Shape* ps, Material* pm) : shape(ps), material(pm){
}

Intersection Primitive::Intersect(const Ray &r) {
    Intersection isect(std::move(shape->Intersect(r)));
    if (isect.intersected())
    {
        isect.material(*material);
    }
    return isect;
}