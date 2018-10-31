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

    template<typename T>
    class RegularGrid final {
    private:
        ::std::vector<::std::vector<Primitive<T> *>> primitives_;
        ::std::int32_t gridSize_{};
        ::std::int32_t gridShift_{};
        AABB m_Extends{};
        ::glm::vec3 m_SR{};
        ::glm::vec3 m_CW{};
        bool hasPrimitives_ {};

    private:
        void addPrimitives(::std::vector<Primitive<T>> &&primitives) noexcept;

        Intersection intersect(Intersection intersection, const Ray &ray, bool shadowTrace = false) noexcept;

    public:
        explicit RegularGrid() noexcept = default;

        explicit RegularGrid<T> (AABB sceneBounds, ::std::vector<Primitive<T>> &&primitives, ::std::int32_t gridSize) noexcept;

        RegularGrid(const RegularGrid &regularGrid) noexcept = delete;

        RegularGrid(RegularGrid &&regularGrid) noexcept = default;

        ~RegularGrid() noexcept;

        RegularGrid &operator=(const RegularGrid &regularGrid) noexcept = delete;

        RegularGrid &operator=(RegularGrid &&regularGrid) noexcept = default;

        Intersection trace(Intersection intersection, const Ray &ray) noexcept;

        Intersection shadowTrace(Intersection intersection, const Ray &ray) noexcept;
    };

    template<typename T>
    RegularGrid<T>::RegularGrid(AABB sceneBounds, ::std::vector<Primitive<T>> &&primitives,
                         const ::std::int32_t gridSize) noexcept :
        primitives_{
                ::std::vector<::std::vector<::MobileRT::Primitive<T>*>> {
                        static_cast<::std::size_t> (gridSize * gridSize * gridSize)}},
        gridSize_{gridSize},
        gridShift_{bitCounter(gridSize) - 1},
        m_Extends(sceneBounds),//world boundaries
        // precalculate 1 / size of a cell (for x, y and z)
        m_SR{gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_)[0],
        gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_)[1],
        gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_)[2]},
        // precalculate size of a cell (for x, y, and z)
        m_CW{(m_Extends.pointMax_ - m_Extends.pointMin_) * (1.0f / gridSize_)},
        hasPrimitives_{primitives.empty()? false : true} {
        LOG("scene min=(", m_Extends.pointMin_[0], ", ", m_Extends.pointMin_[1], ", ",
            m_Extends.pointMin_[2], ") max=(", m_Extends.pointMax_[0], ", ",
            m_Extends.pointMax_[1], ", ", m_Extends.pointMax_[2], ")");

        const ::std::size_t vectorSize{static_cast<::std::size_t> (gridSize * gridSize * gridSize)};
        primitives_.reserve(vectorSize);

        LOG("PRIMITIVES = ", primitives.size());
        LOG("gridSize_ = ", this->gridSize_);
        LOG("gridShift_ = ", this->gridShift_);
        LOG("hasPrimitives_ = ", this->hasPrimitives_);
        addPrimitives(::std::move(primitives));
    }

    template<typename T>
    RegularGrid<T>::~RegularGrid() noexcept {
        primitives_.clear();
        ::std::vector<::std::vector<Primitive<T>*>> {}.swap(primitives_);
    }

    template<typename T>
    void RegularGrid<T>::addPrimitives(::std::vector<Primitive<T>> &&primitives) noexcept {
        ::std::int32_t index{0};

        // calculate cell width, height and depth
        const float sizeX{m_Extends.pointMax_[0] - m_Extends.pointMin_[0]};
        const float sizeY{m_Extends.pointMax_[1] - m_Extends.pointMin_[1]};
        const float sizeZ{m_Extends.pointMax_[2] - m_Extends.pointMin_[2]};
        const float dx{sizeX / gridSize_};
        const float dy{sizeY / gridSize_};
        const float dz{sizeZ / gridSize_};
        const float dx_reci{dx > 0 ? 1.0f / dx : 1.0f};
        const float dy_reci{dy > 0 ? 1.0f / dy : 1.0f};
        const float dz_reci{dz > 0 ? 1.0f / dz : 1.0f};

        // store primitives in the grid cells
        for (Primitive<T> &primitive : primitives) {
            ++index;
            const AABB bound{primitive.getAABB()};
            const ::glm::vec3 &bv1 {bound.pointMin_};
            const ::glm::vec3 &bv2 {bound.pointMax_};

            // find out which cells could contain the primitive (based on aabb)
            ::std::int32_t x1{static_cast<::std::int32_t> ((bv1[0] - m_Extends.pointMin_[0]) * dx_reci)};
            ::std::int32_t x2{static_cast<::std::int32_t> ((bv2[0] - m_Extends.pointMin_[0]) * dx_reci) + 1};
            x1 = (x1 < 0) ? 0 : x1;
            x2 = (x2 > (gridSize_ - 1)) ? gridSize_ - 1 : x2;
            x2 = ::std::fabs(sizeX) < Epsilon? 0 : x2;
            x1 = x1 > x2 ? x2 : x1;
            ::std::int32_t y1{static_cast<::std::int32_t> ((bv1[1] - m_Extends.pointMin_[1]) * dy_reci)};
            ::std::int32_t y2{static_cast<::std::int32_t> ((bv2[1] - m_Extends.pointMin_[1]) * dy_reci) + 1};
            y1 = (y1 < 0) ? 0 : y1;
            y2 = (y2 > (gridSize_ - 1)) ? gridSize_ - 1 : y2;
            y2 = ::std::fabs(sizeY) < Epsilon? 0 : y2;
            y1 = y1 > y2 ? y2 : y1;
            ::std::int32_t z1{static_cast<::std::int32_t> ((bv1[2] - m_Extends.pointMin_[2]) * dz_reci)};
            ::std::int32_t z2{static_cast<::std::int32_t> ((bv2[2] - m_Extends.pointMin_[2]) * dz_reci) + 1};
            z1 = (z1 < 0) ? 0 : z1;
            z2 = (z2 > (gridSize_ - 1)) ? gridSize_ - 1 : z2;
            z2 = ::std::fabs(sizeZ) < Epsilon? 0 : z2;
            z1 = ::std::min(z2, z1);

            //loop over candidate cells
            for (::std::int32_t x{x1}; x <= x2; ++x) {
                for (::std::int32_t y{y1}; y <= y2; ++y) {
                    for (::std::int32_t z{z1}; z <= z2; ++z) {
                        // construct aabb for current cell
                        const ::std::size_t idx {
                                static_cast<::std::size_t> (x) +
                                static_cast<::std::size_t> (y) * static_cast<::std::size_t> (gridSize_) +
                                static_cast<::std::size_t> (z) * static_cast<::std::size_t> (gridSize_) *
                                static_cast<::std::size_t> (gridSize_)};
                        const ::glm::vec3 &pos {m_Extends.pointMin_[0] + x * dx,
                                        m_Extends.pointMin_[1] + y * dy,
                                        m_Extends.pointMin_[2] + z * dz};
                        const AABB &cell {pos, pos + ::glm::vec3 {dx, dy, dz}};
                        //LOG("min=(", pos[0], ", ", pos[1], ", ", pos[2], ") max=(", dx, ", ", dy, ",", dz, ")");
                        // do an accurate aabb / primitive intersection test
                        const bool intersectedBox{::MobileRT::intersectBox(primitive, cell)};
                        if (intersectedBox) {
                            primitives_[idx].emplace_back(&primitive);
                            //LOG("add idx = ", idx, " index = ", index);
                        }
                    }
                }
            }
        }
    }

    template<typename T>
    Intersection RegularGrid<T>::trace(Intersection intersection, const Ray &ray) noexcept {
        if (hasPrimitives_ == true) {
            intersection = intersect(intersection, ray);
        }
        return intersection;
    }

    template<typename T>
    Intersection RegularGrid<T>::shadowTrace(Intersection intersection, const Ray &ray) noexcept {
        if (hasPrimitives_ == true) {
            intersection = intersect(intersection, ray, true);
        }
        return intersection;
    }

    template<typename T>
    Intersection RegularGrid<T>::intersect(Intersection intersection, const Ray &ray,
        const bool shadowTrace) noexcept {
        // setup 3DDDA (double check reusability of primary ray data)
        const ::glm::vec3 &cell {(ray.origin_ - m_Extends.pointMin_) * m_SR};
        ::std::int32_t X{static_cast<::std::int32_t> (cell[0])};
        ::std::int32_t Y{static_cast<::std::int32_t> (cell[1])};
        ::std::int32_t Z{static_cast<::std::int32_t> (cell[2])};
        /*const bool notInGrid{(X < 0) || (X >= gridSize_) ||
                            (Y < 0) || (Y >= gridSize_) ||
                            (Z < 0) || (Z >= gridSize_)};
        if (notInGrid && gridSize_ > 1) {
            return intersection;
        }*/
        X = X < 0? 0 : X;
        Y = Y < 0? 0 : Y;
        Z = Z < 0? 0 : Z;
        X = X >= gridSize_? gridSize_ - 1 : X;
        Y = Y >= gridSize_? gridSize_ - 1 : Y;
        Z = Z >= gridSize_? gridSize_ - 1 : Z;

        ::std::int32_t stepX, outX;
        ::std::int32_t stepY, outY;
        ::std::int32_t stepZ, outZ;
        ::glm::vec3 cb{};
        if (ray.direction_[0] > 0) {
            stepX = 1;
            outX = gridSize_;
            cb[0] = (m_Extends.pointMin_[0] + (X + 1) * m_CW[0]);
        } else {
            stepX = -1;
            outX = -1;
            cb[0] = (m_Extends.pointMin_[0] + X * m_CW[0]);
        }

        if (ray.direction_[1] > 0) {
            stepY = 1;
            outY = gridSize_;
            cb[1] = (m_Extends.pointMin_[1] + (Y + 1) * m_CW[1]);
        } else {
            stepY = -1;
            outY = -1;
            cb[1] = (m_Extends.pointMin_[1] + Y * m_CW[1]);
        }

        if (ray.direction_[2] > 0) {
            stepZ = 1;
            outZ = gridSize_;
            cb[2] = (m_Extends.pointMin_[2] + (Z + 1) * m_CW[2]);
        } else {
            stepZ = -1;
            outZ = -1;
            cb[2] = (m_Extends.pointMin_[2] + Z * m_CW[2]);
        }

        ::glm::vec3 tmax{}, tdelta{};
        if (::std::fabs(ray.direction_[0]) > Epsilon) {
            const float rxr{1.0f / ray.direction_[0]};
            tmax[0] = ((cb[0] - ray.origin_[0]) * rxr);
            tdelta[0] = (m_CW[0] * stepX * rxr);
        } else {
            tmax[0] = (RayLengthMax);
        }

        if (::std::fabs(ray.direction_[1]) > Epsilon) {
            const float ryr{1.0f / ray.direction_[1]};
            tmax[1] = ((cb[1] - ray.origin_[1]) * ryr);
            tdelta[1] = (m_CW[1] * stepY * ryr);
        } else {
            tmax[1] = (RayLengthMax);
        }

        if (::std::fabs(ray.direction_[2]) > Epsilon) {
            const float rzr{1.0f / ray.direction_[2]};
            tmax[2] = ((cb[2] - ray.origin_[2]) * rzr);
            tdelta[2] = (m_CW[2] * stepZ * rzr);
        } else {
            tmax[2] = (RayLengthMax);
        }

        // start stepping
        // trace primary ray
        while (true) {
            const ::std::int32_t index {
                static_cast<int32_t> (
                     static_cast<::std::uint32_t> (X) +
                    (static_cast<::std::uint32_t> (Y) << static_cast<::std::uint32_t> (gridShift_)) +
                    (static_cast<::std::uint32_t> (Z) << (static_cast<::std::uint32_t> (gridShift_) * 2u)))};
            const auto it {this->primitives_.begin() + index};
            ::std::vector<Primitive<T> *> primitivesList {*it};
            for (auto *const primitive : primitivesList) {
                const float lastDist {intersection.length_};
                intersection = primitive->intersect(intersection, ray);
                if (intersection.length_ < lastDist) {
                    if (shadowTrace) {
                        return intersection;
                    }
                    goto testloop;
                }
            }

            if (tmax[0] < tmax[1]) {
                if (tmax[0] < tmax[2]) {
                    X += stepX;
                    if (X == outX) {
                        return intersection;
                    }
                    tmax[0] = (tmax[0] + tdelta[0]);
                } else {
                    Z += stepZ;
                    if (Z == outZ) {
                        return intersection;
                    }
                    tmax[2] = (tmax[2] + tdelta[2]);
                }
            } else {
                if (tmax[1] < tmax[2]) {
                    Y += stepY;
                    if (Y == outY) {
                        return intersection;
                    }
                    tmax[1] = (tmax[1] + tdelta[1]);
                } else {
                    Z += stepZ;
                    if (Z == outZ) {
                        return intersection;
                    }
                    tmax[2] = (tmax[2] + tdelta[2]);
                }
            }

        }
        testloop:
        while (true) {
            const ::std::int32_t index {
                static_cast<int32_t> (
                     static_cast<::std::uint32_t> (X) +
                    (static_cast<::std::uint32_t> (Y) << static_cast<::std::uint32_t> (gridShift_)) +
                    (static_cast<::std::uint32_t> (Z) << (static_cast<::std::uint32_t> (gridShift_) * 2u)))};
            const auto it {this->primitives_.begin() + index};
            ::std::vector<Primitive<T> *> primitivesList {*it};
            for (auto *const primitive : primitivesList) {
                intersection = primitive->intersect(intersection, ray);
            }
            if (tmax[0] < tmax[1]) {
                if (tmax[0] < tmax[2]) {
                    if (intersection.length_ < tmax[0]) {
                        break;
                    }
                    X += stepX;
                    if (X == outX) {
                        break;
                    }
                    tmax[0] = (tmax[0] + tdelta[0]);
                } else {
                    if (intersection.length_ < tmax[2]) {
                        break;
                    }
                    Z += stepZ;
                    if (Z == outZ) {
                        break;
                    }
                    tmax[2] = (tmax[2] + tdelta[2]);
                }
            } else {
                if (tmax[1] < tmax[2]) {
                    if (intersection.length_ < tmax[1]) {
                        break;
                    }
                    Y += stepY;
                    if (Y == outY) {
                        break;
                    }
                    tmax[1] = (tmax[1] + tdelta[1]);
                } else {
                    if (intersection.length_ < tmax[2]) {
                        break;
                    }
                    Z += stepZ;
                    if (Z == outZ) {
                        break;
                    }
                    tmax[2] = (tmax[2] + tdelta[2]);
                }
            }
        }
        return intersection;
    }

}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_REGULARGRID_HPP
