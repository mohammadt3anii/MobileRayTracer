//
// Created by Tiago on 16-10-2016.
//

#include "Intersection.h"

using namespace MobileRT;

static unsigned int counter(0u);

Intersection::Intersection(void) :
        length_(RAY_LENGTH_MAX) {
    counter++;
}

void Intersection::reset(const Point3D &orig, const Vector3D &dir, const float dist,
                         const Vector3D &normal,
                         const Material &material) {
    this->point_.x_ = orig.x_ + dir.x_ * dist;
    this->point_.y_ = orig.y_ + dir.y_ * dist;
    this->point_.z_ = orig.z_ + dir.z_ * dist;
    this->normal_ = normal;
    this->length_ = dist;
    this->material_ = &material;
    this->symNormal_.x_ = -this->normal_.x_;
    this->symNormal_.y_ = -this->normal_.y_;
    this->symNormal_.z_ = -this->normal_.z_;
}

void Intersection::reset(const Point3D &orig, const Vector3D &dir, const float dist,
                         const Point3D &center,
                         const Material &material) {
    this->point_.x_ = orig.x_ + dir.x_ * dist;
    this->point_.y_ = orig.y_ + dir.y_ * dist;
    this->point_.z_ = orig.z_ + dir.z_ * dist;
    this->normal_ = Vector3D(this->point_, center, true);
    this->length_ = dist;
    this->material_ = &material;
    this->symNormal_.x_ = -this->normal_.x_;
    this->symNormal_.y_ = -this->normal_.y_;
    this->symNormal_.z_ = -this->normal_.z_;
}

unsigned int Intersection::getInstances() {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
