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
        const Vector3D AC_;
        const Vector3D AB_;

      const Vector3D BC_;
        const Vector3D normal_;
        const Point3D pointA_;
        const Point3D pointB_;
        const Point3D pointC_;

		private:
				Point3D getPositionMin() const noexcept;

        Point3D getPositionMax() const noexcept;

    public:
			explicit Triangle(const Point3D &pointA, const Point3D &pointB,
                          const Point3D &pointC) noexcept;

			Triangle(const Triangle &triangle) noexcept = default;

			Triangle(Triangle &&triangle) noexcept = default;

			~Triangle() noexcept = default;

			Triangle &operator=(const Triangle &triangle) noexcept = delete;

			Triangle &operator=(Triangle &&triangle) noexcept = delete;

			bool intersect(Intersection *intersection, const Ray &ray) const noexcept;

			void moveTo(float x, float y) noexcept;

			float getZ() const noexcept;

			AABB getAABB() const noexcept;

			bool intersect(const AABB &box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_TRIANGLE_HPP
