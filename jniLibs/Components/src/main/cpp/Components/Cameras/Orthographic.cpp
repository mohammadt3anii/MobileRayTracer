//
// Created by Tiago on 22-Jun-17.
//

#include "Orthographic.h"

using namespace Components;

Orthographic::Orthographic(const Point3D &position, const Point3D &lookAt, const Vector3D &up,
                           const float sizeH, const float sizeV) :
        Camera(position, lookAt, up),
        sizeH_(sizeH),
        sizeV_(sizeV) {
}

Orthographic::~Orthographic(void) {
}

Ray Orthographic::generateRay(const float u, const float v,
                              const float deviationU, const float deviationV) const {
    const float u_alpha((u - 0.5f) * 2.0f + deviationU);
    const float v_alpha((0.5f - v) * 2.0f + deviationV);

    const Point3D position(this->position_ +
                           (this->right_ * (u_alpha * this->sizeH_)) +
                           (this->up_ * (v_alpha * this->sizeV_)));

    return Ray(this->direction_, position, 1);
}
