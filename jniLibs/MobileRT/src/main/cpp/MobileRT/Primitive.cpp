//
// Created by Tiago on 16-10-2016.
//

#include "Primitive.h"

using namespace MobileRT;

static unsigned int counter(0u);

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
}

unsigned int Primitive::getInstances() {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
