//
// Created by puscas on 27/08/17.
//

#ifndef MOBILERT_ACCELERATORS_BVH2_HPP
#define MOBILERT_ACCELERATORS_BVH2_HPP

#include "../Intersection.hpp"
#include "../Scene.hpp"
#include "AABB.hpp"
#include <random>

namespace MobileRT {
  static unsigned counter2 {0};

  template<typename T>
  class BVH2 final {
    public:
      int64_t numberPrimitives_ {0};
      int64_t numberDepth_ {0};
      ::std::vector<::MobileRT::AABB> boxes_ {};
      ::std::vector<::std::vector<MobileRT::Primitive<T>>> primitives_ {};

      #define MAX_DEPTH 999
      #define MAX_VECTOR 1

    private:
      AABB build(::std::vector<MobileRT::Primitive<T>> primitives, const unsigned depth = 0, const uint64_t currentNodeId = 0) noexcept {
        if (primitives.empty()) {
          if (depth == 0) {
            boxes_.emplace_back(AABB{});
          }
          return AABB{};
        }
        if (depth == 0) {
          numberPrimitives_ = static_cast<int64_t>(primitives.size());
        }
        numberDepth_ = depth > numberDepth_? depth : numberDepth_;

        AABB current_box {
          Point3D {RayLengthMax, RayLengthMax, RayLengthMax},
          Point3D {-RayLengthMax, -RayLengthMax, -RayLengthMax}};
        for (uint64_t i {0}; i < primitives.size(); i++) {
          AABB new_box {primitives.at(i).getAABB()};
          current_box = surroundingBox(new_box, current_box);
        }

        const int axis {current_box.getLongestAxis()};
        switch (axis) {
          case 0:
            ::std::sort(primitives.begin(), primitives.end(), [](const MobileRT::Primitive<T>&a, MobileRT::Primitive<T>& b) noexcept -> bool {
              return a.getAABB().pointMin_.x_() < b.getAABB().pointMin_.x_();
            });
            break;

          case 1:
            ::std::sort(primitives.begin(), primitives.end(), [](const MobileRT::Primitive<T>&a, MobileRT::Primitive<T>& b) noexcept -> bool {
              return a.getAABB().pointMin_.y_() < b.getAABB().pointMin_.y_();
            });
            break;

          default:
            ::std::sort(primitives.begin(), primitives.end(), [](const MobileRT::Primitive<T>&a, MobileRT::Primitive<T>& b) noexcept -> bool {
              return a.getAABB().pointMin_.z_() < b.getAABB().pointMin_.z_();
            });
            break;
        }

        int64_t divide {static_cast<int64_t>(primitives.size()) / 2};
        divide = primitives.size() % 2 == 0? divide : divide + 1;

        const uint64_t aux {static_cast<uint64_t>(1 << depth)};
        const uint64_t left {currentNodeId * 2 + 1};
        const uint64_t right {left + 1};

        //LOG("depth = ", depth, ", currentNodeId = ", currentNodeId,", size = ", primitives.size(), ", divide = ", divide);
        //LOG("leftChild = ", left, ", rightChild = ", right);
        //LOG("numberPrimitives_ = ", numberPrimitives_, ", boxesSize = ", boxes_.size());

        if (numberPrimitives_ <= static_cast<int64_t>(aux * 2)) {
          primitives_.emplace_back(primitives);
        } else {
          using Iterator = typename ::std::vector<MobileRT::Primitive<T>>::const_iterator;
          Iterator leftBegin {primitives.begin()};
          Iterator leftEnd {primitives.begin() + divide};
          Iterator rightBegin {primitives.begin() + divide};
          Iterator rightEnd {primitives.end()};

          ::std::vector<MobileRT::Primitive<T>> leftVector(leftBegin, leftEnd);
          ::std::vector<MobileRT::Primitive<T>> rightVector(rightBegin, rightEnd);

          AABB leftBox {build (leftVector, depth + 1, left)};
          AABB rightBox {build (rightVector, depth + 1, right)};
          current_box = surroundingBox(leftBox, rightBox);
        }
        boxes_.at(currentNodeId) = current_box;
        counter2++;
        return current_box;
      }

    public:
      explicit BVH2 () noexcept = default;

      explicit BVH2<T> (::std::vector<MobileRT::Primitive<T>>&& primitives, const unsigned depth = 0, const uint64_t currentId = 0) noexcept {

        if (primitives.empty()) {
          if (depth == 0) {
            boxes_.emplace_back(AABB{});
          }
          return;
        }
        uint64_t numberPrimitives {primitives.size()};
        uint64_t maxDepth {};
        uint64_t maxNodes {};
        for (maxDepth = 1; maxNodes * 2 < numberPrimitives; ) {
          maxDepth++;
          maxNodes = 1 << maxDepth;
        }
        while (maxDepth > 0) {
          maxDepth--;
          maxNodes += 1 << maxDepth;
        }
        //LOG ("maxNodes = ", maxNodes);
        boxes_.resize(maxNodes);

        build(primitives, depth, currentId);
      }

      BVH2 (const BVH2 &bVH) noexcept = delete;

      BVH2 (BVH2 &&bVH) noexcept = default;

      ~BVH2 () noexcept = default;

      BVH2 &operator= (const BVH2 &bVH) noexcept = delete;

      BVH2 &operator= (BVH2 &&bVH) noexcept = default;

      bool trace (::MobileRT::Intersection *const intersection, const ::MobileRT::Ray &ray, const unsigned depth = 0, const uint64_t currentNodeId = 0) noexcept {
        if (boxes_.at(currentNodeId).intersect(ray)) {
          //node at the bottom of tree - no childs
          const uint64_t aux {static_cast<unsigned long>(1 << depth)};
          if (depth == numberDepth_) {
            const uint64_t primitiveId {(currentNodeId - (aux - 1))};
            bool res {false};
            for (MobileRT::Primitive<T> &primitive : primitives_.at(primitiveId)) {
              res |= primitive.intersect(intersection, ray);
            }
            return res;
          }

          const uint64_t left {currentNodeId * 2 + 1};
          const bool hit_left {trace(intersection, ray, depth + 1, left)};
          const bool hit_right {trace(intersection, ray, depth + 1, left + 1)};
          return hit_left || hit_right;
        }
        return false;
      }

      bool shadowTrace (::MobileRT::Intersection *const intersection, const ::MobileRT::Ray &ray, const unsigned depth = 0, const uint64_t currentNodeId = 0) noexcept {
        if (boxes_.at(currentNodeId).intersect(ray)) {
          //node at the bottom of tree - no childs
          const uint64_t aux {static_cast<unsigned long>(1 << depth)};
          if (depth == numberDepth_) {
            const uint64_t primitiveId {(currentNodeId - (aux - 1))};
            for (MobileRT::Primitive<T> &primitive : primitives_.at(primitiveId)) {
              if (primitive.intersect(intersection, ray)) {
                return true;
              }
            }
            return false;
          }

          const uint64_t left {currentNodeId * 2 + 1};
          const bool hit_left {shadowTrace(intersection, ray, depth + 1, left)};
          const bool hit_right {shadowTrace(intersection, ray, depth + 1, left + 1)};
          return hit_left || hit_right;
        }
        return false;
      }
  };
}//namespace MobileRT
#endif //MOBILERT_ACCELERATORS_BVH2_HPP
