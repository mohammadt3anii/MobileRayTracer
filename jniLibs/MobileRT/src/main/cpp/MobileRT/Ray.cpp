//
// Created by Tiago on 16-10-2016.
//

#include "Ray.hpp"

using MobileRT::Ray;

static unsigned int counter(0u);

Ray::Ray(const float x, const float y, const float z, const Point3D &origin,
         const unsigned int depth) noexcept :
        origin_(origin),
        direction_(x, y, z),
        symDirection_(-x, -y, -z),
        depth_(depth) {
    counter++;
}

Ray::Ray(const Vector3D &dir, const Point3D &origin,
         const unsigned int depth) noexcept :
        origin_(origin),
        direction_(dir),
        symDirection_(-dir),
        depth_(depth) {
    counter++;
}

unsigned int Ray::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
