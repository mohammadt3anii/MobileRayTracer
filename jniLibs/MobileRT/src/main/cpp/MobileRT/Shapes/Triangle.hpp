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

			Triangle &operator=(Triangle &&triangle) noexcept = delete;
			bool intersect (Intersection *intersection, Ray ray) const noexcept;

			void moveTo(float x, float y) noexcept;

			float getZ() const noexcept;
      MobileRT::AABB getAABB () const noexcept;
      bool intersect (MobileRT::AABB box) const noexcept;

      void CalculateRange (float &a_Pos1, float &a_Pos2, int a_Axis) {
        //Vector3D pos1 = m_Vertex[0]->GetPos();
        Point3D pos1 {this->pointA_};
        a_Pos1 = pos1.cell[a_Axis];
        a_Pos2 = pos1.cell[a_Axis];
        Point3D pos {this->pointB_};
        if (pos.cell[a_Axis] < a_Pos1) {
          a_Pos1 = pos.cell[a_Axis];
        }
        if (pos.cell[a_Axis] > a_Pos2) {
          a_Pos2 = pos.cell[a_Axis];
        }
        pos = this->pointC_;
        if (pos.cell[a_Axis] < a_Pos1) {
          a_Pos1 = pos.cell[a_Axis];
        }
        if (pos.cell[a_Axis] > a_Pos2) {
          a_Pos2 = pos.cell[a_Axis];
        }
      }
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_TRIANGLE_HPP
