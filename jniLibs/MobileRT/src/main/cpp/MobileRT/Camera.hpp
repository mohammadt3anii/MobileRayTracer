//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_CAMERA_HPP
#define MOBILERT_CAMERA_HPP

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

		Camera(const Camera &camera) noexcept = delete;

		Camera(Camera &&camera) noexcept = delete;

        virtual ~Camera() noexcept;

		Camera &operator=(const Camera &camera) noexcept = delete;

		Camera &operator=(Camera &&camera) noexcept = delete;

        virtual Ray generateRay(float u, float v,
                                      float deviationU,
                                      float deviationV) const noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_CAMERA_HPP
