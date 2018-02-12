//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_VECTOR3D_HPP
#define MOBILERT_VECTOR3D_HPP

#include "Point3D.hpp"
#include <cmath>

namespace MobileRT {
    class Point3D;

    class Vector3D final {

    public:
        float direction_[3]{0.0f};

        float x_() const noexcept { return direction_[0]; }

        float y_() const noexcept { return direction_[1]; }

        float z_() const noexcept { return direction_[2]; }

        void setX(float x) noexcept { direction_[0] = x; }

        void setY(float y) noexcept { direction_[1] = y; }

        void setZ(float z) noexcept { direction_[2] = z; }

        float magnitude_{1.0f};

    private:
        explicit Vector3D(Vector3D vector, float value) noexcept;

        Point3D operator/(float value) const noexcept;

    private:
        float magnitude() const noexcept;

    public:
        explicit Vector3D() noexcept = default;

        explicit Vector3D(float x, float y, float z, float magnitude = 0.0f) noexcept;

        explicit Vector3D(Vector3D vector1, Vector3D vector2, float value) noexcept;

        explicit Vector3D(Point3D dest, Point3D orig, bool /*normalize*/) noexcept;

        explicit Vector3D(Point3D dest, Point3D orig) noexcept;

        explicit Vector3D(Vector3D vector1, Vector3D vector2) noexcept;

        Vector3D(const Vector3D &vector) noexcept = default;

        Vector3D(Vector3D &&vector) noexcept = default;

        ~Vector3D() noexcept = default;

        Vector3D &operator=(const Vector3D &vector) noexcept = default;

        Vector3D &operator=(Vector3D &&vector) noexcept = default;

        Vector3D operator*(float value) const noexcept;

        Vector3D operator+(Vector3D vector) const noexcept;

        Vector3D operator-(Vector3D vector) const noexcept;

        Vector3D operator*(Vector3D vector) const noexcept;

        float dotProduct(Vector3D vector) const noexcept;

        float dotProduct(Point3D dest, Point3D orig) const noexcept;

        float squareMagnitude() const noexcept;

        Vector3D crossProduct(Vector3D vector) const noexcept;

        void operator*=(float value) noexcept;

        Vector3D operator-() const noexcept;

        float normalize() noexcept;

        Vector3D returnNormalized() const noexcept;

        bool isNull() noexcept;

        static unsigned getInstances() noexcept;
    };

    Vector3D operator/(int value, Vector3D vector) noexcept;

    Vector3D operator/(Vector3D vector, int value) noexcept;

}//namespace MobileRT

#endif //MOBILERT_VECTOR3D_HPP
