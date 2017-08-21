//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_POINT3D_HPP
#define MOBILERT_POINT3D_HPP

#include "Vector3D.hpp"

namespace MobileRT {
    class Vector3D;

    class Point3D final {
    public:
      float x_ {};
      float y_ {};
      float z_ {};

    private:
			explicit Point3D (Point3D point, Vector3D vector,
												float value) noexcept;

    public:
			explicit Point3D () noexcept = default;
			explicit Point3D (float x, float y, float z) noexcept;

			Point3D(const Point3D &point) noexcept = default;

			Point3D(Point3D &&point) noexcept = default;

			~Point3D() noexcept = default;

      Point3D &operator=(const Point3D &point3D) noexcept = default;

			Point3D &operator=(Point3D &&point) noexcept = default;
			Vector3D operator- (Point3D point) const noexcept;
			Point3D operator+ (Vector3D vector) const noexcept;
			Point3D operator- (float value) const noexcept;
			Point3D operator+ (float value) const noexcept;
      static unsigned getInstances () noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_POINT3D_HPP
