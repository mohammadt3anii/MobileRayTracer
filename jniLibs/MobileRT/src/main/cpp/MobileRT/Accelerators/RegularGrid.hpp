//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_ACCELERATORS_REGULARGRID_HPP
#define MOBILERT_ACCELERATORS_REGULARGRID_HPP

#include "AABB.hpp"
#include "../Scene.hpp"
#include "../Shapes/Plane.hpp"
#include "../Shapes/Rectangle.hpp"
#include "../Shapes/Sphere.hpp"
#include "../Shapes/Triangle.hpp"
#include <vector>

namespace MobileRT {
    class RegularGrid final {
    private:
      std::vector<std::vector<Sphere*>> spheres_;

      std::vector<std::vector<Triangle *>> triangles_;

      std::vector<std::vector<Plane *>> planes_;
			int gridSize_ {0};
			int gridShift_ {3};
			AABB m_Extends;
			Vector3D m_SR {0, 0, 0};
			Vector3D m_CW;

      private:
      template<typename T>
      void addPrimitives
        (std::vector<T> &primitives, std::vector<std::vector<T *>> &grid_primitives) noexcept;

      template<typename T>
      bool intersect(const std::vector<std::vector<T *>> &primitives, Intersection *intersection,
                     const Ray &ray) const noexcept;

      public:
			explicit RegularGrid(Point3D min, Point3D max, Scene *scene, int gridSize = 8, int gridShift = 3);

			RegularGrid(const RegularGrid &regularGrid) noexcept = delete;

			RegularGrid(RegularGrid &&regularGrid) noexcept = default;

			~RegularGrid() noexcept = default;

			RegularGrid &operator=(const RegularGrid &regularGrid) noexcept = delete;

			RegularGrid &operator=(RegularGrid &&regularGrid) noexcept = default;

			bool intersect(Intersection *intersection, const Ray &ray) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_REGULARGRID_HPP
