//
// Created by Tiago on 16-10-2016.
//

#include "Ray.h"

using namespace MobileRT;

static unsigned int counter = 0;

Ray::Ray(void) :
        depth_(1) {
    counter++;
}

Ray::Ray(const float x, const float y, const float z, const Point3D &origin,
         const unsigned int depth) :
        origin_(origin),
        direction_(x, y, z),
        symDirection_(-x, -y, -z),
        depth_(depth)
{
    counter++;
    this->direction_.normalize();
}

Ray::Ray(const Vector3D &dir, const Point3D &origin,
         const unsigned int depth) :
        origin_(origin),
        direction_(dir),
        symDirection_(dir.symmetric()),
        depth_(depth) {
    counter++;
    this->direction_.normalize();
}

unsigned int Ray::getInstances() {
    const unsigned int res(counter);
    counter = 0;
    return res;
}
