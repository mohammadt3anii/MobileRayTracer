//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_CAMERAS_PERSPECTIVE_HPP
#define COMPONENTS_CAMERAS_PERSPECTIVE_HPP

#include "MobileRT/Camera.hpp"
#include "MobileRT/Utils.hpp"

namespace Components {
	using MobileRT::Point3D;
	using MobileRT::Vector3D;
	using MobileRT::Ray;
    class Perspective final : public MobileRT::Camera {
    private:
        const float hFov_;
        const float vFov_;

    public:
        explicit Perspective(const Point3D &position,
                             const Point3D &lookAt, const Vector3D &up,
                             float hFov, float vFov) noexcept;

        const Ray generateRay(float u, float v,
                                      float deviationU,
                                      float deviationV) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_CAMERAS_PERSPECTIVE_HPP
