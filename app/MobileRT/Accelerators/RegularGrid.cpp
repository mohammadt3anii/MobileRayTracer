//
// Created by Tiago on 6-Mar-17.
//

#include "RegularGrid.hpp"

using ::MobileRT::RegularGrid;
using ::MobileRT::AABB;
using ::MobileRT::Primitive;
using ::MobileRT::Intersection;

RegularGrid::RegularGrid(AABB sceneBounds, Scene *const scene,
                         const ::std::int32_t gridSize) noexcept :
        triangles_{
                ::std::vector<::std::vector<::MobileRT::Primitive<Triangle> *>> {
                        static_cast<size_t> (gridSize * gridSize * gridSize)}},
        spheres_{
                ::std::vector<::std::vector<::MobileRT::Primitive<Sphere> *>> {
                        static_cast<size_t> (gridSize * gridSize * gridSize)}},
        planes_{
                ::std::vector<::std::vector<::MobileRT::Primitive<Plane> *>> {
                        static_cast<size_t> (gridSize * gridSize * gridSize)}},
        rectangles_{
                ::std::vector<::std::vector<::MobileRT::Primitive<Rectangle> *>> {
                        static_cast<size_t> (gridSize * gridSize * gridSize)}},
        gridSize_{gridSize},
        gridShift_{bitCounter(static_cast<::std::uint32_t>(gridSize)) - 1},
        m_Extends(sceneBounds),//world boundaries
        // precalculate 1 / size of a cell (for x, y and z)
        m_SR{gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_).x,
        gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_).y,
        gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_).z},
        // precalculate size of a cell (for x, y, and z)
        m_CW{(m_Extends.pointMax_ - m_Extends.pointMin_) * (1.0f / gridSize_)},
        scene_{scene} {
    LOG("scene min=(", m_Extends.pointMin_.x, ", ", m_Extends.pointMin_.y, ", ",
        m_Extends.pointMin_.z, ") max=(", m_Extends.pointMax_.x, ", ",
        m_Extends.pointMax_.y, ", ", m_Extends.pointMax_.z, ")");

    size_t vectorSize{static_cast<size_t>(gridSize * gridSize * gridSize)};
    triangles_.reserve(vectorSize);
    spheres_.reserve(vectorSize);
    planes_.reserve(vectorSize);
    rectangles_.reserve(vectorSize);

    addPrimitives<Primitive<Triangle>>(scene->triangles_, this->triangles_);
    addPrimitives<Primitive<Sphere>>(scene->spheres_, this->spheres_);
    addPrimitives<Primitive<Plane>>(scene->planes_, this->planes_);
    addPrimitives<Primitive<Rectangle>>(scene->rectangles_, this->rectangles_);
    LOG("TRIANGLES = ", this->triangles_.size());
    LOG("SPHERES = ", this->spheres_.size());
    LOG("PLANES = ", this->planes_.size());
    LOG("RECTANGLES = ", this->rectangles_.size());


    /*for(size_t i {0}; i < this->triangles_.size(); ++i) {
      ::std::vector<Primitive<Triangle>*>& triangles = this->triangles_[i];
      ::std::vector<Primitive<Sphere>*>& spheres = this->spheres_[i];
      ::std::vector<Primitive<Plane>*>& planes = this->planes_[i];
      ::std::vector<Primitive<Rectangle>*>& rectangles = this->rectangles_[i];
      LOG("i = ", i, " -> t = ",  triangles.size(), ", s = ", spheres.size(), ", p = ", planes.size(), " , r = ", rectangles.size(), " size = ", this->triangles_.size());
    }*/
}

::std::int32_t RegularGrid::bitCounter(::std::uint32_t n) const noexcept {
    ::std::int32_t counter{0};
    while (n > 0) {
        ++counter;
        n >>= 1;
    }
    return counter;
}

template<typename T>
void RegularGrid::addPrimitives
        (::std::vector<T> &primitives,
         ::std::vector<::std::vector<T *>> &grid_primitives) noexcept {
    ::std::int32_t index{0};

    // calculate cell width, height and depth
    const float sizeX{m_Extends.pointMax_.x - m_Extends.pointMin_.x};
    const float sizeY{m_Extends.pointMax_.y - m_Extends.pointMin_.y};
    const float sizeZ{m_Extends.pointMax_.z - m_Extends.pointMin_.z};
    const float dx{sizeX / gridSize_};
    const float dy{sizeY / gridSize_};
    const float dz{sizeZ / gridSize_};
    const float dx_reci{dx > 0 ? 1.0f / dx : 1.0f};
    const float dy_reci{dy > 0 ? 1.0f / dy : 1.0f};
    const float dz_reci{dz > 0 ? 1.0f / dz : 1.0f};

    // store primitives in the grid cells
    for (T &primitive : primitives) {
        ++index;
        const AABB bound{primitive.getAABB()};
        const ::glm::vec3 bv1{bound.pointMin_};
        const ::glm::vec3 bv2{bound.pointMax_};

        // find out which cells could contain the primitive (based on aabb)
        ::std::int32_t x1{static_cast<::std::int32_t>((bv1.x - m_Extends.pointMin_.x) * dx_reci)};
        ::std::int32_t x2{static_cast<::std::int32_t>((bv2.x - m_Extends.pointMin_.x) * dx_reci) + 1};
        x1 = (x1 < 0) ? 0 : x1;
        x2 = (x2 > (gridSize_ - 1)) ? gridSize_ - 1 : x2;
        x2 = sizeX == 0 ? 0 : x2;
        x1 = x1 > x2 ? x2 : x1;
        ::std::int32_t y1{static_cast<::std::int32_t>((bv1.y - m_Extends.pointMin_.y) * dy_reci)};
        ::std::int32_t y2{static_cast<::std::int32_t>((bv2.y - m_Extends.pointMin_.y) * dy_reci) + 1};
        y1 = (y1 < 0) ? 0 : y1;
        y2 = (y2 > (gridSize_ - 1)) ? gridSize_ - 1 : y2;
        y2 = sizeY == 0 ? 0 : y2;
        y1 = y1 > y2 ? y2 : y1;
        ::std::int32_t z1{static_cast<::std::int32_t>((bv1.z - m_Extends.pointMin_.z) * dz_reci)};
        ::std::int32_t z2{static_cast<::std::int32_t>((bv2.z - m_Extends.pointMin_.z) * dz_reci) + 1};
        z1 = (z1 < 0) ? 0 : z1;
        z2 = (z2 > (gridSize_ - 1)) ? gridSize_ - 1 : z2;
        z2 = sizeZ == 0 ? 0 : z2;
        z1 = ::std::min(z2, z1);

        //loop over candidate cells
        for (::std::int32_t x{x1}; x <= x2; ++x) {
            for (::std::int32_t y{y1}; y <= y2; ++y) {
                for (::std::int32_t z{z1}; z <= z2; ++z) {
                    // construct aabb for current cell
                    const size_t idx{
                            static_cast<size_t>(x) +
                            static_cast<size_t>(y) * static_cast<size_t>(gridSize_) +
                            static_cast<size_t>(z) * static_cast<size_t>(gridSize_) *
                            static_cast<size_t>(gridSize_)};
                    const ::glm::vec3 pos{m_Extends.pointMin_.x + x * dx,
                                      m_Extends.pointMin_.y + y * dy,
                                      m_Extends.pointMin_.z + z * dz};
                    const AABB cell{pos, pos + ::glm::vec3 {dx, dy, dz}};
                    //LOG("min=(", pos.x_, ", ", pos.y_, ", ", pos.z_, ") max=(", dx, ", ", dy, ",", dz, ")");
                    // do an accurate aabb / primitive intersection test
                    const bool intersectedBox{::MobileRT::intersect(primitive, cell)};
                    if (intersectedBox) {
                        grid_primitives[idx].emplace_back(&primitive);
                        //LOG("add idx = ", idx, " index = ", index);
                    }
                }
            }
        }
    }
}

Intersection RegularGrid::trace(Intersection intersection, Ray ray) noexcept {
    intersection =
            intersect<::MobileRT::Primitive<Triangle>>(this->triangles_, intersection, ray);
    intersection =
            intersect<::MobileRT::Primitive<Sphere>>(this->spheres_, intersection, ray);
    intersection =
            intersect<::MobileRT::Primitive<Plane>>(this->planes_, intersection, ray);
    intersection =
            intersect<::MobileRT::Primitive<Rectangle>>(this->rectangles_, intersection, ray);
    intersection = this->scene_->traceLights(intersection, ray);
    return intersection;
}

Intersection RegularGrid::shadowTrace(Intersection intersection, Ray ray) noexcept {
    intersection =
            intersect<::MobileRT::Primitive<Triangle>>(this->triangles_, intersection, ray, true);
    intersection =
            intersect<::MobileRT::Primitive<Sphere>>(this->spheres_, intersection, ray, true);
    intersection =
            intersect<::MobileRT::Primitive<Plane>>(this->planes_, intersection, ray, true);
    intersection =
            intersect<::MobileRT::Primitive<Rectangle>>(this->rectangles_, intersection, ray,
                                                        true);
    return intersection;
}

template<typename T>
Intersection RegularGrid::intersect(const ::std::vector<::std::vector<T *>> &primitivesMatrix,
                            Intersection intersection, const Ray ray,
                            const bool shadowTrace) noexcept {
    // setup 3DDDA (double check reusability of primary ray data)
    const ::glm::vec3 cell{(ray.origin_ - m_Extends.pointMin_) * m_SR};
    ::std::int32_t X{static_cast<::std::int32_t>(cell.x)};
    ::std::int32_t Y{static_cast<::std::int32_t>(cell.y)};
    ::std::int32_t Z{static_cast<::std::int32_t>(cell.z)};
    const bool notInGrid{(X < 0) || (X >= gridSize_) ||
                         (Y < 0) || (Y >= gridSize_) ||
                         (Z < 0) || (Z >= gridSize_)};
    if (notInGrid && gridSize_ > 1) {
        return intersection;
    }

    ::std::int32_t stepX, outX;
    ::std::int32_t stepY, outY;
    ::std::int32_t stepZ, outZ;
    ::glm::vec3 cb{};
    if (ray.direction_.x > 0) {
        stepX = 1;
        outX = gridSize_;
        cb.x = (m_Extends.pointMin_.x + (X + 1) * m_CW.x);
    } else {
        stepX = -1;
        outX = -1;
        cb.x = (m_Extends.pointMin_.x + X * m_CW.x);
    }

    if (ray.direction_.y > 0) {
        stepY = 1;
        outY = gridSize_;
        cb.y = (m_Extends.pointMin_.y + (Y + 1) * m_CW.y);
    } else {
        stepY = -1;
        outY = -1;
        cb.y = (m_Extends.pointMin_.y + Y * m_CW.y);
    }

    if (ray.direction_.z > 0) {
        stepZ = 1;
        outZ = gridSize_;
        cb.z = (m_Extends.pointMin_.z + (Z + 1) * m_CW.z);
    } else {
        stepZ = -1;
        outZ = -1;
        cb.z = (m_Extends.pointMin_.z + Z * m_CW.z);
    }

    ::glm::vec3 tmax{}, tdelta{};
    if (ray.direction_.x != 0) {
        const float rxr{1.0f / ray.direction_.x};
        tmax.x = ((cb.x - ray.origin_.x) * rxr);
        tdelta.x = (m_CW.x * stepX * rxr);
    } else {
        tmax.x = (RayLengthMax);
    }

    if (ray.direction_.y != 0) {
        const float ryr{1.0f / ray.direction_.y};
        tmax.y = ((cb.y - ray.origin_.y) * ryr);
        tdelta.y = (m_CW.y * stepY * ryr);
    } else {
        tmax.y = (RayLengthMax);
    }

    if (ray.direction_.z != 0) {
        const float rzr{1.0f / ray.direction_.z};
        tmax.z = ((cb.z - ray.origin_.z) * rzr);
        tdelta.z = (m_CW.z * stepZ * rzr);
    } else {
        tmax.z = (RayLengthMax);
    }

    // start stepping
    // trace primary ray
    while (true) {
        const size_t index{
                static_cast<size_t>(X) +
                (static_cast<size_t>(Y) << gridShift_) +
                (static_cast<size_t>(Z) << (gridShift_ * 2))};
        ::std::vector<T *> primitivesList{primitivesMatrix[index]};
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

        if (tmax.x < tmax.y) {
            if (tmax.x < tmax.z) {
                X += stepX;
                if (X == outX) {
                    return intersection;
                }
                tmax.x = (tmax.x + tdelta.x);
            } else {
                Z += stepZ;
                if (Z == outZ) {
                    return intersection;
                }
                tmax.z = (tmax.z + tdelta.z);
            }
        } else {
            if (tmax.y < tmax.z) {
                Y += stepY;
                if (Y == outY) {
                    return intersection;
                }
                tmax.y = (tmax.y + tdelta.y);
            } else {
                Z += stepZ;
                if (Z == outZ) {
                    return intersection;
                }
                tmax.z = (tmax.z + tdelta.z);
            }
        }

    }
    testloop:
    while (true) {
        const size_t index{static_cast<size_t>(X) +
                           (static_cast<size_t>(Y) << gridShift_) +
                           (static_cast<size_t>(Z) << (gridShift_ * 2))};
        ::std::vector<T *> primitivesList{primitivesMatrix[index]};
        for (auto *const primitive : primitivesList) {
            intersection = primitive->intersect(intersection, ray);
        }
        if (tmax.x < tmax.y) {
            if (tmax.x < tmax.z) {
                if (intersection.length_ < tmax.x) {
                    break;
                }
                X += stepX;
                if (X == outX) {
                    break;
                }
                tmax.x = (tmax.x + tdelta.x);
            } else {
                if (intersection.length_ < tmax.z) {
                    break;
                }
                Z += stepZ;
                if (Z == outZ) {
                    break;
                }
                tmax.z = (tmax.z + tdelta.z);
            }
        } else {
            if (tmax.y < tmax.z) {
                if (intersection.length_ < tmax.y) {
                    break;
                }
                Y += stepY;
                if (Y == outY) {
                    break;
                }
                tmax.y = (tmax.y + tdelta.y);
            } else {
                if (intersection.length_ < tmax.z) {
                    break;
                }
                Z += stepZ;
                if (Z == outZ) {
                    break;
                }
                tmax.z = (tmax.z + tdelta.z);
            }
        }
    }
    return intersection;
}
