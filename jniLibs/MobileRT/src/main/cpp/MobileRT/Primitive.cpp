//
// Created by Tiago on 16-10-2016.
//

#include "Primitive.h"

using namespace MobileRT;

static unsigned int counter = 0;

Primitive::Primitive(Shape *shape, const Material &material) :
        material_(material),
        shape_(shape) {
    counter++;
}

Primitive::~Primitive(void) {
    delete this->shape_;
}

bool Primitive::intersect(Intersection &intersection, const Ray &ray) const {
    return this->shape_->intersect(intersection, ray, this->material_);
    /*if (this->shape_->intersect(intersection, ray, this->material_))
    {
        return true;
    }
    return false;*/
}

unsigned int Primitive::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}