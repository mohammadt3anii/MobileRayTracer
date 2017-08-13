//
// Created by Tiago on 6-Mar-17.
//

#include "RegularGrid.hpp"

using MobileRT::RegularGrid;

RegularGrid::RegularGrid (const Point3D min, const Point3D max, Scene *const scene,
                          const int gridSize,
                          const int gridShift) :
  spheres_ {
    std::vector<std::vector<Sphere *>> {static_cast<size_t> (gridSize * gridSize * gridSize)}},
  triangles_ {
    std::vector<std::vector<Triangle *>> {static_cast<size_t> (gridSize * gridSize * gridSize)}},
  planes_ {
    std::vector<std::vector<Plane *>> {static_cast<size_t> (gridSize * gridSize * gridSize)}},
  gridSize_ {gridSize},
  gridShift_ {gridShift},
  m_Extends (AABB {min, max}),
  // precalculate 1 / size of a cell (for x, y and z)
  m_SR {gridSize_ / (m_Extends.pointMax_ - m_Extends.pointMin_)},
  // precalculate size of a cell (for x, y, and z)
  m_CW {(m_Extends.pointMax_ - m_Extends.pointMin_) * (1.0f / gridSize_)}
{
  LOG("scene min=(", m_Extends.pointMin_.x_, ", ", m_Extends.pointMin_.y_, ", ", m_Extends.pointMin_.z_, ") max=(", m_Extends.pointMax_.x_, ", ", m_Extends.pointMax_.y_, ", ", m_Extends.pointMax_.z_, ")");
  LOG("SPHERES");
  addPrimitives<Sphere>(scene->spheres_, this->spheres_);
  LOG("TRIANGLES");
  addPrimitives<Triangle>(scene->triangles_, this->triangles_);
  LOG("PLANES");
  addPrimitives<Plane>(scene->planes_, this->planes_);
}

template<typename T>
void RegularGrid::addPrimitives
  (std::vector<T> &primitives, std::vector<std::vector<T *>> &grid_primitives) noexcept {
  int index {0};
  for (std::vector<T *> primitivesBox : grid_primitives) {
    primitivesBox = std::vector<T *>();
  }

  const float dx {(m_Extends.pointMax_.x_ - m_Extends.pointMin_.x_) / gridSize_};
  const float dy {(m_Extends.pointMax_.y_ - m_Extends.pointMin_.y_) / gridSize_};
  const float dz {(m_Extends.pointMax_.z_ - m_Extends.pointMin_.z_) / gridSize_};
  const float dx_reci {1.0f / dx};
  const float dy_reci {1.0f / dy};
  const float dz_reci {1.0f / dz};

  // store primitives in the grid cells
  for (T &primitive : primitives) {
    index ++;
    const AABB bound {primitive.getAABB ()};
    const Point3D bv1 {bound.pointMin_};
    const Point3D bv2 {bound.pointMax_};

    // find out which cells could contain the primitive (based on aabb)
    int x1 {static_cast<int>((bv1.x_ - m_Extends.pointMin_.x_) * dx_reci)};
    int x2 {static_cast<int>((bv2.x_ - m_Extends.pointMin_.x_) * dx_reci) + 1};
    x1 = (x1 < 0) ? 0 : x1;
    x2 = (x2 > (gridSize_ - 1)) ? gridSize_ - 1 : x2;
    int y1 {static_cast<int>((bv1.y_ - m_Extends.pointMin_.y_) * dy_reci)};
    int y2 {static_cast<int>((bv2.y_ - m_Extends.pointMin_.y_) * dy_reci) + 1};
    y1 = (y1 < 0) ? 0 : y1;
    y2 = (y2 > (gridSize_ - 1)) ? gridSize_ - 1 : y2;
    int z1 {static_cast<int>((bv1.z_ - m_Extends.pointMin_.z_) * dz_reci)};
    int z2 {static_cast<int>((bv2.z_ - m_Extends.pointMin_.z_) * dz_reci) + 1};
    z1 = (z1 < 0) ? 0 : z1;
    z2 = (z2 > (gridSize_ - 1)) ? gridSize_ - 1 : z2;

    for (int x {x1}; x < x2; x ++) {
      for (int y {y1}; y < y2; y ++) {
        for (int z {z1}; z < z2; z ++) {
          // construct aabb for current cell
          const size_t idx {
            static_cast<size_t>(x) + static_cast<size_t>(y) * static_cast<size_t>(gridSize_) +
            static_cast<size_t>(z) * static_cast<size_t>(gridSize_) *
            static_cast<size_t>(gridSize_)};
          const Point3D pos {m_Extends.pointMin_.x_ + x * dx,
                             m_Extends.pointMin_.y_ + y * dy,
                             m_Extends.pointMin_.z_ + z * dz};
          const AABB cell {pos, Point3D {dx, dy, dz}};
          LOG("index = ", index);
          LOG("min=(", pos.x_, ", ", pos.y_, ", ", pos.z_, ") max=(", dx, ", ", dy, ",", dz, ")");
          // do an accurate aabb / primitive intersection test
          if (primitive.intersect(cell)) {
            grid_primitives[idx].emplace_back(&primitive);
          }
        }
      }
    }
  }
}

bool RegularGrid::intersect (Intersection *const intersection, const Ray &ray) const noexcept {
  const bool intersectedSpheres {intersect<Sphere> (this -> spheres_, intersection, ray)};
  const bool intersectedTriangles {intersect<Triangle> (this -> triangles_, intersection, ray)};
  const bool intersectedPlanes {intersect<Plane> (this -> planes_, intersection, ray)};
  return intersectedSpheres || intersectedTriangles || intersectedPlanes;
}

template<typename T>
bool RegularGrid::intersect(const std::vector<std::vector<T *>> &primitives,
                            Intersection *const intersection, const Ray ray) const noexcept {
  bool retval {false};
  const Vector3D &raydir {ray.direction_};
  const Point3D &curpos {ray.origin_};
  const AABB &e(m_Extends);
  // setup 3DDDA (double check reusability of primary ray data)
  Vector3D cb {}, tmax {}, tdelta {};
  Vector3D cell {(curpos - e.pointMin_) * m_SR};
  int stepX, outX, X {static_cast<int>(cell.x_)};
  int stepY, outY, Y {static_cast<int>(cell.y_)};
  int stepZ, outZ, Z {static_cast<int>(cell.z_)};

  if ((X < 0) || (X >= gridSize_) || (Y < 0) || (Y >= gridSize_) || (Z < 0) || (Z >= gridSize_)) {
    return false;
  }

  if (raydir.x_ > 0) {
    stepX = 1;
    outX = gridSize_;
    cb.x_ = e.pointMin_.x_ + (X + 1) * m_CW.x_;
  } else {
    stepX = - 1;
    outX = - 1;
    cb.x_ = e.pointMin_.x_ + X * m_CW.x_;
  }

  if (raydir.y_ > 0.0f) {
    stepY = 1;
    outY = gridSize_;
    cb.y_ = e.pointMin_.y_ + (Y + 1) * m_CW.y_;
  } else {
    stepY = - 1;
    outY = - 1;
    cb.y_ = e.pointMin_.y_ + Y * m_CW.y_;
  }

  if (raydir.z_ > 0.0f) {
    stepZ = 1;
    outZ = gridSize_;
    cb.z_ = e.pointMin_.z_ + (Z + 1) * m_CW.z_;
  } else {
    stepZ = - 1;
    outZ = - 1;
    cb.z_ = e.pointMin_.z_ + Z * m_CW.z_;
  }

  if (raydir.x_ != 0.0f) {
    const float rxr {1.0f / raydir.x_};
    tmax.x_ = (cb.x_ - curpos.x_) * rxr;
    tdelta.x_ = m_CW.x_ * stepX * rxr;
  } else {
    tmax.x_ = 1000000;
  }

  if (raydir.y_ != 0.0f) {
    const float ryr {1.0f / raydir.y_};
    tmax.y_ = (cb.y_ - curpos.y_) * ryr;
    tdelta.y_ = m_CW.y_ * stepY * ryr;
  } else {
    tmax.y_ = 1000000;
  }

  if (raydir.z_ != 0.0f) {
    const float rzr {1.0f / raydir.z_};
    tmax.z_ = (cb.z_ - curpos.z_) * rzr;
    tdelta.z_ = m_CW.z_ * stepZ * rzr;
  } else {
    tmax.z_ = 1000000;
  }

  // start stepping
  // trace primary ray
  while (true) {
    const size_t index {static_cast<size_t>(X) + (static_cast<size_t>(Y) << gridShift_) +
                        (static_cast<size_t>(Z) << (static_cast<size_t>(gridShift_) * 2))};
    std::vector<T *> list {primitives[index]};

    for (auto *pr : list) {
      bool result {false};
      // if (pr->GetLastRayID() != a_Ray.GetID()) {
      result = pr->intersect(intersection, ray);
      if (result) {
        retval = result;
        goto testloop;
      }
      // }
    }

    if (tmax.x_ < tmax.y_) {
      if (tmax.x_ < tmax.z_) {
        X = X + stepX;
        if (X == outX) {
          return false;
        }
        tmax.x_ += tdelta.x_;
      } else {
        Z = Z + stepZ;
        if (Z == outZ) {
          return false;
        }
        tmax.z_ += tdelta.z_;
      }
    } else {
      if (tmax.y_ < tmax.z_) {
        Y = Y + stepY;
        if (Y == outY) {
          return false;
        }
        tmax.y_ += tdelta.y_;
      } else {
        Z = Z + stepZ;
        if (Z == outZ) {
          return false;
        }
        tmax.z_ += tdelta.z_;
      }
    }

  }

  testloop:
  while (true) {
    const size_t index {static_cast<size_t>(X) +
                        (static_cast<size_t>(Y) << static_cast<size_t>(gridShift_)) +
                        (static_cast<size_t>(Z) << (static_cast<size_t>(gridShift_) * 2))};
    std::vector<T *> list {primitives[index]};

    for (auto *pr : list) {
      bool result {false};
      // if (pr->GetLastRayID() != a_Ray.GetID()) {
      result = pr->intersect(intersection, ray);
      if (result) {
        retval = result;
      }
      // }
    }
    if (tmax.x_ < tmax.y_) {
      if (tmax.x_ < tmax.z_) {
        if (intersection->length_ < tmax.x_) {
          break;
        }
        X = X + stepX;
        if (X == outX) {
          break;
        }
        tmax.x_ += tdelta.x_;
      } else {
        if (intersection->length_ < tmax.z_) {
          break;
        }
        Z = Z + stepZ;
        if (Z == outZ) {
          break;
        }
        tmax.z_ += tdelta.z_;
      }
    } else {
      if (tmax.y_ < tmax.z_) {
        if (intersection->length_ < tmax.y_) {
          break;
        }
        Y = Y + stepY;
        if (Y == outY) {
          break;
        }
        tmax.y_ += tdelta.y_;
      } else {
        if (intersection->length_ < tmax.z_) {
          break;
        }
        Z = Z + stepZ;
        if (Z == outZ) {
          break;
        }
        tmax.z_ += tdelta.z_;
      }
    }
  }

  return retval;
}
