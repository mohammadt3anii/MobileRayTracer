//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_CAMERA_H
#define MOBILERAYTRACER_CAMERA_H

#include "Point3D.hpp"
#include "Ray.hpp"

namespace MobileRT {
    class Camera {
    public:
        Point3D position_;
        Vector3D direction_;
        Vector3D right_;
        Vector3D up_;

    public:
        explicit Camera(const Point3D &position,
                        const Point3D &lookAt, const Vector3D &up) noexcept;

        virtual ~Camera() noexcept;

        virtual const Ray generateRay(float u, float v,
                                      float deviationU,
                                      float deviationV) const noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_CAMERA_H
