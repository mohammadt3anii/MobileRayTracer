//
// Created by Tiago on 15-11-2016.
//

#ifndef MOBILERT_SHAPES_TRIANGLE_HPP
#define MOBILERT_SHAPES_TRIANGLE_HPP

#include "../Accelerators/AABB.hpp"
#include "../Intersection.hpp"
#include "../Point3D.hpp"
#include "../Ray.hpp"
#include "../Vector3D.hpp"

namespace MobileRT {
    class Triangle final {
    public:
      Vector3D AC_ {};
      Vector3D AB_ {};
      Vector3D BC_ {};
      Vector3D normal_ {};
      Point3D pointA_ {};
      Point3D pointB_ {};
      Point3D pointC_ {};

		private:
				Point3D getPositionMin() const noexcept;

        Point3D getPositionMax() const noexcept;

    public:
			explicit Triangle (Point3D pointA, Point3D pointB,
												 Point3D pointC, Vector3D normal = Vector3D {}) noexcept;

			Triangle(const Triangle &triangle) noexcept = default;

			Triangle(Triangle &&triangle) noexcept = default;

			~Triangle() noexcept = default;
      
      Triangle &operator= (const Triangle &triangle) noexcept = default;

			Triangle &operator=(Triangle &&triangle) noexcept = default;

			bool intersect (Intersection *intersection, Ray ray) const noexcept;

			void moveTo(float x, float y) noexcept;

			float getZ() const noexcept;

      ::MobileRT::AABB getAABB () const noexcept;

      bool intersect (::MobileRT::AABB box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_TRIANGLE_HPP
