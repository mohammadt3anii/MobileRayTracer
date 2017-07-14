//
// Created by Tiago on 16-10-2016.
//

#include "Primitive.hpp"

using MobileRT::Primitive;

static unsigned int counter(0u);

Primitive::Primitive(Shape *shape, const Material &material) noexcept :
        material_(material),
        shape_(shape) {
    counter++;
}

Primitive::~Primitive() noexcept {
    delete this->shape_;
}

bool Primitive::intersect(Intersection &intersection, const Ray &ray) const noexcept {
    return this->shape_->intersect(intersection, ray, this->material_);
}

unsigned int Primitive::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
