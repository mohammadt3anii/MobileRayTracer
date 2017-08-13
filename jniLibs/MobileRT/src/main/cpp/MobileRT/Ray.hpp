//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_RAY_HPP
#define MOBILERT_RAY_HPP

#include "Point3D.hpp"
#include "Utils.hpp"
#include "Vector3D.hpp"

namespace MobileRT {
    class Ray final {
    public:
      const Point3D origin_ {};
      const Vector3D direction_ {};
      const unsigned depth_ {};

    public:
      explicit Ray(Vector3D dir, Point3D origin,
                   unsigned depth) noexcept;
      Ray (const Ray &ray) noexcept = default;
      Ray (Ray &&ray) noexcept = default;
      ~Ray () noexcept = default;
      Ray &operator= (const Ray &ray) noexcept = delete;
      Ray &operator= (Ray &&ray) noexcept = delete;
      static unsigned getInstances () noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_RAY_HPP
