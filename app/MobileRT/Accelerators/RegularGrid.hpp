//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_ACCELERATORS_REGULARGRID_HPP
#define MOBILERT_ACCELERATORS_REGULARGRID_HPP

#include "MobileRT/Accelerators/AABB.hpp"
#include "MobileRT/Primitive.hpp"
#include "MobileRT/Scene.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace MobileRT {
    using MobileRT::Primitive;

    class RegularGrid final {
    private:
        ::std::vector<::std::vector<Primitive<Triangle> *>> triangles_;
        ::std::vector<::std::vector<Primitive<Sphere> *>> spheres_;
        ::std::vector<::std::vector<Primitive<Plane> *>> planes_;
        ::std::vector<::std::vector<Primitive<Rectangle> *>> rectangles_;
        int gridSize_{};
        int gridShift_{};
        AABB m_Extends{};
        ::glm::vec3 m_SR{};
        ::glm::vec3 m_CW{};
        Scene *scene_{};

    private:
        template<typename T>
        void addPrimitives
                (::std::vector<T> &primitives,
                 ::std::vector<::std::vector<T *>> &grid_primitives) noexcept;

        template<typename T>
        Intersection intersect(const ::std::vector<::std::vector<T *>> &primitivesMatrix,
                       Intersection intersection,
                       Ray ray, bool shadowTrace = false) noexcept;

        int bitCounter(unsigned int n) const noexcept;

    public:
        explicit RegularGrid() noexcept = default;

        explicit RegularGrid(AABB sceneBounds, Scene *scene, int gridSize) noexcept;

        RegularGrid(const RegularGrid &regularGrid) noexcept = delete;

        RegularGrid(RegularGrid &&regularGrid) noexcept = default;

        ~RegularGrid() noexcept = default;

        RegularGrid &operator=(const RegularGrid &regularGrid) noexcept = delete;

        RegularGrid &operator=(RegularGrid &&regularGrid) noexcept = default;

        Intersection trace(Intersection intersection, Ray ray) noexcept;

        Intersection shadowTrace(Intersection intersection, Ray ray) noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_REGULARGRID_HPP
