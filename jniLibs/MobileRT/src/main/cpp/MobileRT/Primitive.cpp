//
// Created by Tiago on 16-10-2016.
//

#include "Primitive.hpp"

using MobileRT::Primitive;

static unsigned int counter(0u);

Primitive::Primitive(Shape *shape, Material const &material) noexcept :
        shape_(shape),
        material_(material) {
    counter++;
}

Primitive::Primitive(Primitive &&primitive) noexcept :
        shape_(primitive.shape_),
        material_(primitive.material_) {
    primitive.shape_ = nullptr;
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
