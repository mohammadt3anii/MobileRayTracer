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
      float x_ {0.0f};
      float y_ {0.0f};
      float z_ {0.0f};
      float magnitude_ {1.0f};

    private:
			explicit Vector3D(const Vector3D &vector, float value) noexcept;

			Point3D operator/(float value) const noexcept;

      private:
      float magnitude() const noexcept;

    public:
      explicit Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f,
                        float magnitude = 0.0f) noexcept;

			explicit Vector3D(const Vector3D &vector1, const Vector3D &vector2,
												float value) noexcept;

			explicit Vector3D(const Point3D &dest, const Point3D &orig, bool) noexcept;

			explicit Vector3D(const Point3D &dest, const Point3D &orig) noexcept;

			explicit Vector3D(const Vector3D &vector1, const Vector3D &vector2) noexcept;

			Vector3D(const Vector3D &vector) noexcept = default;

			Vector3D(Vector3D &&vector) noexcept = default;

			~Vector3D() noexcept = default;

			Vector3D &operator=(const Vector3D &vector) noexcept = default;

			Vector3D &operator=(Vector3D &&vector) noexcept = default;

			Vector3D operator*(float value) const noexcept;

			Vector3D operator+(const Vector3D &vector) const noexcept;

			Vector3D operator-(const Vector3D &vector) const noexcept;

			Vector3D operator*(const Vector3D &vector) const noexcept;

			float dotProduct(const Vector3D &vector) const noexcept;

			float dotProduct(const Point3D &dest, const Point3D &orig) const noexcept;

			float squareMagnitude() const noexcept;

			Vector3D crossProduct(const Vector3D &vector) const noexcept;

			void operator*=(float value) noexcept;

			Vector3D operator-() const noexcept;

			float normalize() noexcept;

			static unsigned int getInstances() noexcept;
    };

  Vector3D operator/(int value, const Vector3D &vector) noexcept;

}//namespace MobileRT

#endif //MOBILERT_VECTOR3D_HPP
