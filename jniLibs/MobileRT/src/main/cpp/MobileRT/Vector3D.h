//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_VECTOR3D_H
#define MOBILERAYTRACER_VECTOR3D_H

#include "Point3D.h"
#include <cmath>

namespace MobileRT {
    class Point3D;

    class Vector3D final {
    public:
        float x_;
        float y_;
        float z_;
        float magnitude_;

    private:
        explicit Vector3D(const Vector3D &vector, const float value) noexcept;

        float magnitude(void) const noexcept;

        const Point3D operator/(const float value) const noexcept;

    public:
        explicit Vector3D(void) noexcept;

        explicit Vector3D(const float x, const float y, const float z) noexcept;

        explicit Vector3D(const Vector3D &vector1, const Vector3D &vector2,
                          const float value) noexcept;

        explicit Vector3D(const Point3D &dest, const Point3D &orig, bool) noexcept;

        explicit Vector3D(const Point3D &dest, const Point3D &orig) noexcept;

        explicit Vector3D(const Vector3D &vector1, const Vector3D &vector2) noexcept;

        Vector3D(const Vector3D &vector) noexcept;

        const Vector3D &operator=(const Vector3D &vector) noexcept;

        const Vector3D operator*(const float value) noexcept;

        const Vector3D operator+(const Vector3D &vector) const noexcept;

        float dotProduct(const Vector3D &vector) const noexcept;

        float dotProduct(const Point3D &dest, const Point3D &orig) const noexcept;

        float squareMagnitude(void) const noexcept;

        const Vector3D crossProduct(const Vector3D &vector) const noexcept;

        void mult(const float value) noexcept;

        const Vector3D returnNormalized(void) const noexcept;

        const Vector3D operator*(const float value) const noexcept;

        const Vector3D symmetric(void) const noexcept;

        static unsigned int getInstances() noexcept;
    };
}

#endif //MOBILERAYTRACER_VECTOR3D_H
