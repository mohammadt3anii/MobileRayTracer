//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAY_H
#define MOBILERAYTRACER_RAY_H

#include "Point3D.hpp"
#include "Utils.hpp"
#include "Vector3D.hpp"

namespace MobileRT {
    class Ray final {
    public:
        const Point3D origin_;
        const Vector3D direction_;
        const Vector3D symDirection_;
        const unsigned int depth_;

    public:
        explicit Ray(float x, float y, float z,
                     const Point3D &origin,
                     unsigned int depth) noexcept;

        explicit Ray(const Vector3D &dir, const Point3D &origin,
                     unsigned int depth) noexcept;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_RAY_H
