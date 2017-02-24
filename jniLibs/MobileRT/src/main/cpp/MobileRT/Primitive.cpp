//
// Created by Tiago on 16-10-2016.
//

#include "Primitive.h"

using namespace MobileRT;

static unsigned int counter = 0;

Primitive::Primitive(Plane &plane, const Material &material) :
        material_(material),
        shape_(&plane),
        plane_(&plane),
        sphere_(nullptr),
        triangle_(nullptr),
        intersect([this](Intersection &intersection, const Ray &ray) {
            return this->plane_->intersect(intersection, ray, this->material_);
        }) {
    counter++;
}

Primitive::Primitive(Sphere &sphere, const Material &material) :
        material_(material),
        shape_(&sphere),
        plane_(nullptr),
        sphere_(&sphere),
        triangle_(nullptr),
        intersect([this](Intersection &intersection, const Ray &ray) {
            return this->sphere_->intersect(intersection, ray, this->material_);
        }) {
    counter++;
}

Primitive::Primitive(Triangle &triangle, const Material &material) :
        material_(material),
        shape_(&triangle),
        plane_(nullptr),
        sphere_(nullptr),
        triangle_(&triangle),
        intersect([this](Intersection &intersection, const Ray &ray) {
            return this->triangle_->intersect(intersection, ray, this->material_);
        }) {
    counter++;
}

Primitive::Primitive(Shape &shape, const Material &material) :
        material_(material),
        shape_(&shape),
        plane_(nullptr),
        sphere_(nullptr),
        triangle_(nullptr) {
    counter++;
}

Primitive::~Primitive(void) {
    delete this->shape_;
    delete this->plane_;
    delete this->sphere_;
    delete this->triangle_;
}

/*bool Primitive::intersect(Intersection &intersection, const Ray &ray) const {
    return this->intersect_(intersection, ray);
    //return this->shape_->intersect(intersection, ray, this->material_);
}*/

unsigned int Primitive::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}
