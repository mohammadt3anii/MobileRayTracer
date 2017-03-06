//
// Created by Tiago on 16-10-2016.
//

#include "Intersection.h"

using namespace MobileRT;

static unsigned int counter = 0;

Intersection::Intersection(void) :
        material_(nullptr),
        length_(RAY_LENGTH_MAX) {
    counter++;
}

void Intersection::reset(const Point3D &point,
                         const Vector3D &normal, const float length, const Material &material) {
    this->point_ = point;
    this->normal_ = normal;
    this->length_ = length;
    this->material_ = &material;
}

void Intersection::reset(const float x, const float y, const float z,
                         const Vector3D &normal, const float length, const Material &material) {
    this->point_.x_ = x;
    this->point_.y_ = y;
    this->point_.z_ = z;
    this->normal_ = normal;
    this->length_ = length;
    this->material_ = &material;
}

void Intersection::reset(const Point3D &orig, const Vector3D &dir, const float dist,
                         const Vector3D &normal, const float length, const Material &material) {
    this->point_.x_ = orig.x_ + dir.x_ * dist;
    this->point_.y_ = orig.y_ + dir.y_ * dist;
    this->point_.z_ = orig.z_ + dir.z_ * dist;
    this->normal_ = normal;
    this->length_ = length;
    this->material_ = &material;
}

Vector3D &Intersection::getSymNormal(void) {
    this->symNormal_.reset(-this->normal_.x_, -this->normal_.y_, -this->normal_.z_);
    return this->symNormal_;
}

unsigned int Intersection::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}
