//
// Created by Tiago on 6-Mar-17.
//

#include "RegularGrid.hpp"

using ::MobileRT::RegularGrid;
using ::MobileRT::AABB;
using ::MobileRT::Primitive;

RegularGrid::RegularGrid(AABB sceneBounds, Scene *const scene,
                         const int gridSize) noexcept :
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
        gridShift_{bitCounter(static_cast<unsigned>(gridSize)) - 1},
        m_Extends(sceneBounds),//world boundaries
        // precalculate 1 / size of a cell (for x, y and z)
        m_SR{gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_)},
        // precalculate size of a cell (for x, y, and z)
        m_CW{(m_Extends.pointMax_ - m_Extends.pointMin_) * (1.0f / gridSize_)},
        scene_{scene} {
    LOG("scene min=(", m_Extends.pointMin_.x_(), ", ", m_Extends.pointMin_.y_(), ", ",
        m_Extends.pointMin_.z_(), ") max=(", m_Extends.pointMax_.x_(), ", ",
        m_Extends.pointMax_.y_(), ", ", m_Extends.pointMax_.z_(), ")");

    addPrimitives<Primitive<Triangle>>(scene->triangles_, this->triangles_);
    addPrimitives<Primitive<Sphere>>(scene->spheres_, this->spheres_);
    addPrimitives<Primitive<Plane>>(scene->planes_, this->planes_);
    addPrimitives<Primitive<Rectangle>>(scene->rectangles_, this->rectangles_);
    LOG("TRIANGLES = ", this->triangles_.size());
    LOG("SPHERES = ", this->spheres_.size());
    LOG("PLANES = ", this->planes_.size());
    LOG("RECTANGLES = ", this->rectangles_.size());


    /*for(size_t i {0}; i < this->triangles_.size(); i++) {
      ::std::vector<Primitive<Triangle>*>& triangles = this->triangles_[i];
      ::std::vector<Primitive<Sphere>*>& spheres = this->spheres_[i];
      ::std::vector<Primitive<Plane>*>& planes = this->planes_[i];
      ::std::vector<Primitive<Rectangle>*>& rectangles = this->rectangles_[i];
      LOG("i = ", i, " -> t = ",  triangles.size(), ", s = ", spheres.size(), ", p = ", planes.size(), " , r = ", rectangles.size(), " size = ", this->triangles_.size());
    }*/
}

int RegularGrid::bitCounter(unsigned int n) const noexcept {
    int counter{0};
    while (n > 0) {
        counter++;
        n >>= 1;
    }
    return counter;
}

template<typename T>
void RegularGrid::addPrimitives
        (::std::vector<T> &primitives,
         ::std::vector<::std::vector<T *>> &grid_primitives) noexcept {
    int index{0};

    // calculate cell width, height and depth
    const float sizeX{m_Extends.pointMax_.x_() - m_Extends.pointMin_.x_()};
    const float sizeY{m_Extends.pointMax_.y_() - m_Extends.pointMin_.y_()};
    const float sizeZ{m_Extends.pointMax_.z_() - m_Extends.pointMin_.z_()};
    const float dx{sizeX / gridSize_};
    const float dy{sizeY / gridSize_};
    const float dz{sizeZ / gridSize_};
    const float dx_reci{dx > 0 ? 1.0f / dx : 1.0f};
    const float dy_reci{dy > 0 ? 1.0f / dy : 1.0f};
    const float dz_reci{dz > 0 ? 1.0f / dz : 1.0f};

    // store primitives in the grid cells
    for (T &primitive : primitives) {
        index++;
        const AABB bound{primitive.getAABB()};
        const Point3D bv1{bound.pointMin_};
        const Point3D bv2{bound.pointMax_};

        // find out which cells could contain the primitive (based on aabb)
        int x1{static_cast<int>((bv1.x_() - m_Extends.pointMin_.x_()) * dx_reci)};
        int x2{static_cast<int>((bv2.x_() - m_Extends.pointMin_.x_()) * dx_reci) + 1};
        x1 = (x1 < 0) ? 0 : x1;
        x2 = (x2 > (gridSize_ - 1)) ? gridSize_ - 1 : x2;
        x2 = sizeX == 0 ? 0 : x2;
        x1 = x1 > x2 ? x2 : x1;
        int y1{static_cast<int>((bv1.y_() - m_Extends.pointMin_.y_()) * dy_reci)};
        int y2{static_cast<int>((bv2.y_() - m_Extends.pointMin_.y_()) * dy_reci) + 1};
        y1 = (y1 < 0) ? 0 : y1;
        y2 = (y2 > (gridSize_ - 1)) ? gridSize_ - 1 : y2;
        y2 = sizeY == 0 ? 0 : y2;
        y1 = y1 > y2 ? y2 : y1;
        int z1{static_cast<int>((bv1.z_() - m_Extends.pointMin_.z_()) * dz_reci)};
        int z2{static_cast<int>((bv2.z_() - m_Extends.pointMin_.z_()) * dz_reci) + 1};
        z1 = (z1 < 0) ? 0 : z1;
        z2 = (z2 > (gridSize_ - 1)) ? gridSize_ - 1 : z2;
        z2 = sizeZ == 0 ? 0 : z2;
        z1 = ::std::min(z2, z1);

        //loop over candidate cells
        for (int x{x1}; x <= x2; x++) {
            for (int y{y1}; y <= y2; y++) {
                for (int z{z1}; z <= z2; z++) {
                    // construct aabb for current cell
                    const size_t idx{
                            static_cast<size_t>(x) +
                            static_cast<size_t>(y) * static_cast<size_t>(gridSize_) +
                            static_cast<size_t>(z) * static_cast<size_t>(gridSize_) *
                            static_cast<size_t>(gridSize_)};
                    const Point3D pos{m_Extends.pointMin_.x_() + x * dx,
                                      m_Extends.pointMin_.y_() + y * dy,
                                      m_Extends.pointMin_.z_() + z * dz};
                    const AABB cell{pos, pos + Vector3D {dx, dy, dz}};
                    //LOG("min=(", pos.x_, ", ", pos.y_, ", ", pos.z_, ") max=(", dx, ", ", dy, ",", dz, ")");
                    // do an accurate aabb / primitive intersection test
                    const bool intersectedBox{primitive.intersect(cell)};
                    if (intersectedBox) {
                        grid_primitives[idx].emplace_back(&primitive);
                        //LOG("add idx = ", idx, " index = ", index);
                    }
                }
            }
        }
    }
}

bool RegularGrid::trace(Intersection *const intersection, const Ray &ray) noexcept {
    bool intersectedTriangles{
            intersect<::MobileRT::Primitive<Triangle>>(this->triangles_, intersection, ray)};
    bool intersectedSpheres{
            intersect<::MobileRT::Primitive<Sphere>>(this->spheres_, intersection, ray)};
    bool intersectedPlanes{
            intersect<::MobileRT::Primitive<Plane>>(this->planes_, intersection, ray)};
    bool intersectedRectangles{
            intersect<::MobileRT::Primitive<Rectangle>>(this->rectangles_, intersection, ray)};
    bool intersectedLights{this->scene_->traceLights(intersection, ray)};
    return intersectedTriangles || intersectedSpheres || intersectedPlanes ||
           intersectedRectangles || intersectedLights;
}

bool RegularGrid::shadowTrace(Intersection *const intersection, Ray &&ray) noexcept {
    bool intersectedTriangles{
            intersect<::MobileRT::Primitive<Triangle>>(this->triangles_, intersection, ray, true)};
    bool intersectedSpheres{
            intersect<::MobileRT::Primitive<Sphere>>(this->spheres_, intersection, ray, true)};
    bool intersectedPlanes{
            intersect<::MobileRT::Primitive<Plane>>(this->planes_, intersection, ray, true)};
    bool intersectedRectangles{
            intersect<::MobileRT::Primitive<Rectangle>>(this->rectangles_, intersection, ray,
                                                        true)};
    return intersectedTriangles || intersectedSpheres || intersectedPlanes || intersectedRectangles;
}

template<typename T>
bool RegularGrid::intersect(const ::std::vector<::std::vector<T *>> &primitivesMatrix,
                            Intersection *const intersection, const Ray ray,
                            const bool shadowTrace) noexcept {
    bool retval{false};
    // setup 3DDDA (double check reusability of primary ray data)
    const Vector3D cell{(ray.origin_ - m_Extends.pointMin_) * m_SR};
    int X{static_cast<int>(cell.x_())};
    int Y{static_cast<int>(cell.y_())};
    int Z{static_cast<int>(cell.z_())};
    const bool notInGrid{(X < 0) || (X >= gridSize_) ||
                         (Y < 0) || (Y >= gridSize_) ||
                         (Z < 0) || (Z >= gridSize_)};
    if (notInGrid && gridSize_ > 1) {
        return false;
    }

    int stepX, outX;
    int stepY, outY;
    int stepZ, outZ;
    Vector3D cb{};
    if (ray.direction_.x_() > 0) {
        stepX = 1;
        outX = gridSize_;
        cb.setX(m_Extends.pointMin_.x_() + (X + 1) * m_CW.x_());
    } else {
        stepX = -1;
        outX = -1;
        cb.setX(m_Extends.pointMin_.x_() + X * m_CW.x_());
    }

    if (ray.direction_.y_() > 0) {
        stepY = 1;
        outY = gridSize_;
        cb.setY(m_Extends.pointMin_.y_() + (Y + 1) * m_CW.y_());
    } else {
        stepY = -1;
        outY = -1;
        cb.setY(m_Extends.pointMin_.y_() + Y * m_CW.y_());
    }

    if (ray.direction_.z_() > 0) {
        stepZ = 1;
        outZ = gridSize_;
        cb.setZ(m_Extends.pointMin_.z_() + (Z + 1) * m_CW.z_());
    } else {
        stepZ = -1;
        outZ = -1;
        cb.setZ(m_Extends.pointMin_.z_() + Z * m_CW.z_());
    }

    Vector3D tmax{}, tdelta{};
    if (ray.direction_.x_() != 0) {
        const float rxr{1.0f / ray.direction_.x_()};
        tmax.setX((cb.x_() - ray.origin_.x_()) * rxr);
        tdelta.setX(m_CW.x_() * stepX * rxr);
    } else {
        tmax.setX(RayLengthMax);
    }

    if (ray.direction_.y_() != 0) {
        const float ryr{1.0f / ray.direction_.y_()};
        tmax.setY((cb.y_() - ray.origin_.y_()) * ryr);
        tdelta.setY(m_CW.y_() * stepY * ryr);
    } else {
        tmax.setY(RayLengthMax);
    }

    if (ray.direction_.z_() != 0) {
        const float rzr{1.0f / ray.direction_.z_()};
        tmax.setZ((cb.z_() - ray.origin_.z_()) * rzr);
        tdelta.setZ(m_CW.z_() * stepZ * rzr);
    } else {
        tmax.setZ(RayLengthMax);
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
            if (primitive->intersect(intersection, ray)) {
                retval = true;
                if (shadowTrace) {
                    return retval;
                }
                goto testloop;
            }
        }

        if (tmax.x_() < tmax.y_()) {
            if (tmax.x_() < tmax.z_()) {
                X += stepX;
                if (X == outX) {
                    return false;
                }
                tmax.setX(tmax.x_() + tdelta.x_());
            } else {
                Z += stepZ;
                if (Z == outZ) {
                    return false;
                }
                tmax.setZ(tmax.z_() + tdelta.z_());
            }
        } else {
            if (tmax.y_() < tmax.z_()) {
                Y += stepY;
                if (Y == outY) {
                    return false;
                }
                tmax.setY(tmax.y_() + tdelta.y_());
            } else {
                Z += stepZ;
                if (Z == outZ) {
                    return false;
                }
                tmax.setZ(tmax.z_() + tdelta.z_());
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
            retval |= primitive->intersect(intersection, ray);
        }
        if (tmax.x_() < tmax.y_()) {
            if (tmax.x_() < tmax.z_()) {
                if (intersection->length_ < tmax.x_()) {
                    break;
                }
                X += stepX;
                if (X == outX) {
                    break;
                }
                tmax.setX(tmax.x_() + tdelta.x_());
            } else {
                if (intersection->length_ < tmax.z_()) {
                    break;
                }
                Z += stepZ;
                if (Z == outZ) {
                    break;
                }
                tmax.setZ(tmax.z_() + tdelta.z_());
            }
        } else {
            if (tmax.y_() < tmax.z_()) {
                if (intersection->length_ < tmax.y_()) {
                    break;
                }
                Y += stepY;
                if (Y == outY) {
                    break;
                }
                tmax.setY(tmax.y_() + tdelta.y_());
            } else {
                if (intersection->length_ < tmax.z_()) {
                    break;
                }
                Z += stepZ;
                if (Z == outZ) {
                    break;
                }
                tmax.setZ(tmax.z_() + tdelta.z_());
            }
        }
    }
    return retval;
}
