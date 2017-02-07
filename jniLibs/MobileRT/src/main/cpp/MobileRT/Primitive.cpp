//
// Created by Tiago on 16-10-2016.
//

#include "Primitive.h"

using namespace MobileRT;

Primitive::Primitive(Shape &shape, const Material &material) :
        material_(material),
        shape_(&shape) {
}

Primitive::~Primitive() {
}

bool Primitive::intersect(Intersection &intersection, const Ray &ray) const {
    return this->shape_->intersect(intersection, ray, this->material_);
}