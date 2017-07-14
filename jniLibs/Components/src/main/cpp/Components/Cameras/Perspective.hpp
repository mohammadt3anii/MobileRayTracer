//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PERSPECTIVE_H
#define MOBILERAYTRACER_PERSPECTIVE_H

#include "MobileRT/Camera.hpp"
#include "MobileRT/Utils.hpp"

using MobileRT::Point3D;
using MobileRT::Vector3D;
using MobileRT::Ray;
namespace Components {
    class Perspective final : public MobileRT::Camera {
    private:
        const float hFov_;
        const float vFov_;

    public:
        explicit Perspective(const Point3D &position,
                             const Point3D &lookAt, const Vector3D &up,
                             float hFov, float vFov) noexcept;

        virtual ~Perspective() noexcept override final;

        virtual const Ray generateRay(float x, float y,
                                      float deviationU,
                                      float deviationV) const noexcept override final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_PERSPECTIVE_H
