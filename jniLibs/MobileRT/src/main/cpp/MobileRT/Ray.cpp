//
// Created by Tiago on 16-10-2016.
//

#include "Ray.h"

using namespace MobileRT;

static unsigned int counter = 0;

Ray::Ray(void) :
        maxDistance_(RAY_LENGTH_MAX),
        depth_(1) {
    counter++;
}

Ray::Ray(const float x, const float y, const float z, const Point3D &origin,
         const unsigned int depth) :
        origin_(origin),
        direction_(x, y, z),
        symDirection_(-x, -y, -z),
        maxDistance_(RAY_LENGTH_MAX),
        depth_(depth) {
    counter++;
    this->direction_.normalize();
}

Ray::Ray(Point3D &orig, const Vector3D &dir, const float maxDist, const unsigned int depth) :
        origin_(orig),
        direction_(dir),
        maxDistance_(maxDist),
        depth_(depth) {
    counter++;
    this->direction_.normalize();
}

void Ray::reset(const float x, const float y, const float z, const Point3D &origin) {
    this->direction_.reset(x, y, z);
    this->symDirection_.reset(-x, -y, -z);
    this->origin_ = origin;
}

unsigned int Ray::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}
