//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PERSPECTIVE_H
#define MOBILERAYTRACER_PERSPECTIVE_H

#include "MobileRT/Camera.h"
#include "MobileRT/Utils.h"

namespace Components {
    using namespace MobileRT;

    class Perspective final : public Camera {
    private:
        const float hFov_;
        const float vFov_;

    public:
        explicit Perspective(const Point3D &position,
                             const Point3D &lookAt, const Vector3D &up,
                             const float hFov, const float vFov) noexcept;

        virtual ~Perspective(void) noexcept override final;

        virtual const Ray generateRay(const float x, const float y,
                                      const float deviationU,
                                      const float deviationV) const noexcept override final;
    };
}

#endif //MOBILERAYTRACER_PERSPECTIVE_H
