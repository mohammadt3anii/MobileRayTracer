//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_ACCELERATORS_REGULARGRID_HPP
#define MOBILERT_ACCELERATORS_REGULARGRID_HPP

#include "AABB.hpp"
#include "../Primitive.hpp"
#include "../Scene.hpp"
#include "../Shapes/Plane.hpp"
#include "../Shapes/Rectangle.hpp"
#include "../Shapes/Sphere.hpp"
#include "../Shapes/Triangle.hpp"
#include <vector>

namespace MobileRT {
    class RegularGrid final {
    private:
      std::vector<std::vector<Primitive<Triangle> *>> triangles_ {
        std::vector<std::vector<Primitive<Triangle> *>> {}};
      std::vector<std::vector<Primitive<Sphere> *>> spheres_ {
        std::vector<std::vector<Primitive<Sphere> *>> {}};
      std::vector<std::vector<Primitive<Plane> *>> planes_ {
        std::vector<std::vector<Primitive<Plane> *>> {}};
			std::vector<std::vector<Primitive<Rectangle> *>> rectangles_ {
				std::vector<std::vector<Primitive<Rectangle> *>> {}};
      int gridSize_ {};
      int gridShift_ {};
      AABB m_Extends {};
      Vector3D m_SR {};
      Vector3D m_CW {};
      Scene *scene_ {};
      private:
      template<typename T>
      void addPrimitives
        (std::vector<T> &primitives, std::vector<std::vector<T *>> &grid_primitives) noexcept;

      template<typename T>
      bool intersect(const std::vector<std::vector<T *>> &primitives, Intersection *intersection,
                     Ray ray, bool shadowTrace = false) const noexcept;
      int bitCounter (unsigned int n) const noexcept;
      public:
      explicit RegularGrid () = default;
      explicit RegularGrid (AABB sceneBounds, Scene *scene, int gridSize);

			RegularGrid(const RegularGrid &regularGrid) noexcept = delete;

			RegularGrid(RegularGrid &&regularGrid) noexcept = default;

			~RegularGrid() noexcept = default;

			RegularGrid &operator=(const RegularGrid &regularGrid) noexcept = delete;

			RegularGrid &operator=(RegularGrid &&regularGrid) noexcept = default;

			bool trace(Intersection *intersection, const Ray &ray) const noexcept;
      bool shadowTrace (Intersection *intersection, Ray &&ray) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_REGULARGRID_HPP
