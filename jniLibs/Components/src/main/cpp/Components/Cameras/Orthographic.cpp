//
// Created by Tiago on 22-Jun-17.
//

#include "Orthographic.h"

using namespace Components;

Orthographic::Orthographic(const Point3D &position, const Point3D &lookAt, const Vector3D &up,
                           const float sizeH, const float sizeV) noexcept :
        Camera(position, lookAt, up),
        sizeH_(sizeH / 2.0f),
        sizeV_(sizeV / 2.0f) {
}

Orthographic::~Orthographic(void) noexcept {
}

const Ray Orthographic::generateRay(const float u, const float v,
                                    const float deviationU, const float deviationV) const noexcept {
    const Point3D position(this->position_ +
                           (this->right_ * ((u - 0.5f) * 2.0f + deviationU * this->sizeH_)) +
                           (this->up_ * ((0.5f - v) * 2.0f + deviationV * this->sizeV_)));

    return Ray(this->direction_, position, 1u);
}
