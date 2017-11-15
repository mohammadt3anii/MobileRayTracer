//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHAPES_PLANE_HPP
#define MOBILERT_SHAPES_PLANE_HPP

#include "../Accelerators/AABB.hpp"
#include "../Intersection.hpp"
#include "../Ray.hpp"

namespace MobileRT {
    class Plane final {
    private:
      const Vector3D normal_ {};    // normal to the plane
      const Point3D point_ {};   // point in the plane

		private:
				Point3D getPositionMin() const noexcept;

        Point3D getPositionMax() const noexcept;
      Vector3D getRightVector () const noexcept;

    public:
      explicit Plane (Point3D point, Vector3D normal) noexcept;

			Plane(const Plane &plane) noexcept = default;

			Plane(Plane &&plane) noexcept = default;

      ~Plane() noexcept = default;
      Plane &operator= (const Plane &plane) noexcept = default;

			Plane &operator=(Plane &&plane) noexcept = delete;
      bool intersect (Intersection *intersection, Ray ray) const noexcept;

      void moveTo(float x, float y) noexcept;

      float getZ() const noexcept;

			AABB getAABB() const noexcept;
      float distance (Point3D point) const noexcept;
      bool intersect (AABB box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_PLANE_HPP
