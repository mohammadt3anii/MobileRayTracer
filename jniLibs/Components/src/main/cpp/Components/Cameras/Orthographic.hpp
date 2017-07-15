//
// Created by Tiago on 22-Jun-17.
//

#ifndef MOBILERAYTRACER_ORTHOGRAPHIC_H
#define MOBILERAYTRACER_ORTHOGRAPHIC_H

#include "MobileRT/Camera.hpp"

namespace Components {
	using MobileRT::Point3D;
	using MobileRT::Vector3D;
	using MobileRT::Ray;
    class Orthographic final : public MobileRT::Camera {
    private:
        const float sizeH_;
        const float sizeV_;

    public:
        explicit Orthographic(const Point3D &position,
                              const Point3D &lookAt, const Vector3D &up,
                              float sizeH, float sizeV) noexcept;

        const Ray generateRay(float u, float v,
                                      float deviationU,
                                      float deviationV) const noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_ORTHOGRAPHIC_H
