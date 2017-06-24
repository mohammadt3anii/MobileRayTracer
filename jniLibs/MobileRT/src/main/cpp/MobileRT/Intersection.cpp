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

Intersection::Intersection(Intersection &intersection) :
        point_(intersection.point_),
        normal_(intersection.normal_),
        symNormal_(intersection.symNormal_),
        material_(intersection.material_),
        length_(intersection.length_),
        origin_(intersection.origin_)
{
    this->symNormal_.reset(-this->normal_.x_ + 0.0f, -this->normal_.y_ + 0.0f, -this->normal_.z_ + 0.0f);
    counter++;
}

void Intersection::reset(const Point3D &point,
                         const Vector3D &normal, const float length, const Material &material,
                         const Point3D &origin) {
    this->point_ = point;
    this->normal_ = normal;
    this->length_ = length;
    this->material_ = &material;
    this->symNormal_.reset(-this->normal_.x_ + 0.0f, -this->normal_.y_ + 0.0f, -this->normal_.z_ + 0.0f);
    this->origin_ = origin;
}

void Intersection::reset(const float x, const float y, const float z,
                         const Vector3D &normal, const float length, const Material &material,
                         const Point3D &origin) {
    this->point_.x_ = x;
    this->point_.y_ = y;
    this->point_.z_ = z;
    this->normal_ = normal;
    this->length_ = length;
    this->material_ = &material;
    this->symNormal_.reset(-this->normal_.x_ + 0.0f, -this->normal_.y_ + 0.0f, -this->normal_.z_ + 0.0f);
    this->origin_ = origin;
}

void Intersection::reset(const Point3D &orig, const Vector3D &dir, const float dist,
                         const Vector3D &normal, const Material &material,
                         const Point3D &origin) {
    this->point_.x_ = orig.x_ + dir.x_ * dist;
    this->point_.y_ = orig.y_ + dir.y_ * dist;
    this->point_.z_ = orig.z_ + dir.z_ * dist;
    this->normal_ = normal;
    this->length_ = dist;
    this->material_ = &material;
    this->symNormal_.reset(-this->normal_.x_ + 0.0f, -this->normal_.y_ + 0.0f, -this->normal_.z_ + 0.0f);
    this->origin_ = origin;
}

void Intersection::reset(Intersection &intersection) {
    this->point_ = intersection.point_;
    this->normal_ = intersection.normal_;
    this->length_ = intersection.length_;
    this->material_ = intersection.material_;
    this->symNormal_.reset(-this->normal_.x_ + 0.0f, -this->normal_.y_ + 0.0f, -this->normal_.z_ + 0.0f);
    this->origin_ = intersection.origin_;
}

unsigned int Intersection::getInstances() {
    const unsigned int res(counter);
    counter = 0;
    return res;
}
