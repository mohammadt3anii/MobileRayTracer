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
			//Scene *scene_;
			int gridSize_ {0};
			int gridShift_ {3};
			AABB m_Extends;
			Vector3D m_SR;
			Vector3D m_CW;

    public:
			explicit RegularGrid(Point3D min, Point3D max, Scene *scene, int gridSize = 8, int gridShift = 3);

			RegularGrid(const RegularGrid &regularGrid) noexcept = delete;

			RegularGrid(RegularGrid &&regularGrid) noexcept = default;

			~RegularGrid() noexcept = default;

			RegularGrid &operator=(const RegularGrid &regularGrid) noexcept = default;

			RegularGrid &operator=(RegularGrid &&regularGrid) noexcept = default;

			bool intersect(Intersection *intersection, const Ray &ray) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_REGULARGRID_HPP
