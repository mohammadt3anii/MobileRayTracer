//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINT3D_H
#define MOBILERAYTRACER_POINT3D_H

#include "Vector3D.hpp"

namespace MobileRT {
    class Vector3D;

    class Point3D final {
    public:
        float x_;
        float y_;
        float z_;

    private:
        explicit Point3D(const Point3D &point, const Vector3D &vector,
                         float value) noexcept;

    public:
        explicit Point3D(float x, float y, float z) noexcept;

		Point3D(const Point3D &point) noexcept = default;

		Point3D(Point3D &&point) noexcept = default;

		~Point3D() noexcept = default;

        Point3D &operator=(const Point3D &point3D) noexcept = default;

		Point3D &&operator=(Point3D &&point) const noexcept = delete;

        const Vector3D operator-(const Point3D &point) const noexcept;

        const Point3D operator+(const Vector3D &vector) const noexcept;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_POINT3D_H
