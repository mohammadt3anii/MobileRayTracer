//
// Created by puscas on 27/08/17.
//

#ifndef MOBILERT_ACCELERATORS_BVH_HPP
#define MOBILERT_ACCELERATORS_BVH_HPP

#include "AABB.hpp"
#include "../Intersection.hpp"
#include "../Scene.hpp"

using ::MobileRT::Scene;
using ::MobileRT::AABB;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;
namespace MobileRT {
  //template<typename T> 
  class BVH final {
    public:
      BVH *left_ {nullptr};
      BVH *right_ {nullptr};
      AABB box_ {};
      std::vector<MobileRT::Primitive<MobileRT::Sphere>> spheres_ {};

    public:
      explicit BVH () noexcept = default;

      explicit BVH (AABB sceneBounds, std::vector<MobileRT::Primitive<MobileRT::Sphere>> spheres) noexcept;

      BVH (const BVH &bVH) noexcept = delete;

      BVH (BVH &&bVH) noexcept = default;

      ~BVH () noexcept = default;

      BVH &operator= (const BVH &bVH) noexcept = delete;

      BVH &operator= (BVH &&bVH) noexcept = default;

      bool trace (Intersection *intersection, const Ray &ray) noexcept;

      bool shadowTrace (Intersection *intersection, Ray &&ray) noexcept;
  };
}//namespace MobileRT
#endif //MOBILERT_ACCELERATORS_BVH_HPP
