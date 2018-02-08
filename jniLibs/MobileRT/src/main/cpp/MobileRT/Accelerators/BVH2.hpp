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
      ::std::vector<::MobileRT::AABB> boxes_ {};
      ::std::vector<::std::vector<MobileRT::Primitive<T>>> primitives_ {};

      #define MAX_DEPTH 999
      #define MAX_VECTOR 1

    public:
      explicit BVH2 () noexcept = default;

      explicit BVH2<T> (::std::vector<MobileRT::Primitive<T>> primitives, const unsigned depth = 0, const int64_t currentId = 0) noexcept {
        static int64_t primitiveId {0};
        if (primitives.empty()) {
          if (depth == 0) {
            boxes_.emplace_back(AABB{});
          }
          return;
        }

        const uint64_t currentNode {static_cast<uint64_t>(currentId)};
        if (currentNode == 0) {//if root copy primitives
          primitives_.emplace_back(primitives);

          ::std::vector<MobileRT::Primitive<T>> &primitivesCurrent = primitives_.at(currentNode);
          LOG("primitives_[0] = ", primitivesCurrent.size());
        }

        AABB current_box {};
        for (uint64_t i {0}; i < primitives.size(); i++) {
          AABB new_box {primitives.at(i).getAABB()};
          current_box = surroundingBox(new_box, current_box);
        }
        boxes_.emplace_back(current_box);

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

        const int64_t divide {static_cast<int64_t>(primitives.size()) / 2};
        const unsigned numberOfNodesDepth {static_cast<unsigned>(2 << depth)};
        int64_t left {currentId + numberOfNodesDepth + 0};
        int64_t right {currentId + numberOfNodesDepth + 1};

        LOG("depth = ", depth, ", currentNode = ", currentNode,", size = ", primitives.size(), ", divide = ", divide, ", numberOfNodesDepth = ", numberOfNodesDepth);
        LOG("leftChild = ", left, ", rightChild = ", right);

        if (primitives.size() <= MAX_VECTOR || depth >= MAX_DEPTH) {
          primitives_.emplace_back(primitives);
          right = std::abs(primitiveId);

          LOG("primitiveId = ", primitiveId, ", leftChild = ", left, ", right_ = ", right, ", boxesSize = ", boxes_.size());
          primitiveId++;
        } else {
          using Iterator = typename ::std::vector<MobileRT::Primitive<T>>::const_iterator;
          Iterator leftBegin {primitives.begin()};
          Iterator leftEnd {primitives.begin() + divide};
          Iterator rightBegin {primitives.begin() + divide};
          Iterator rightEnd {primitives.end()};

          ::std::vector<MobileRT::Primitive<T>> leftVector(leftBegin, leftEnd);
          ::std::vector<MobileRT::Primitive<T>> rightVector(rightBegin, rightEnd);

          BVH2 (leftVector, depth + 1, left);
          BVH2 (rightVector, depth + 1, right);
        }
        counter2++;
      }

      BVH2 (const BVH2 &bVH) noexcept = delete;

      BVH2 (BVH2 &&bVH) noexcept = default;

      ~BVH2 () noexcept = default;

      BVH2 &operator= (const BVH2 &bVH) noexcept = delete;

      BVH2 &operator= (BVH2 &&bVH) noexcept = default;

      bool trace (::MobileRT::Intersection *const intersection, const ::MobileRT::Ray &ray, const unsigned depth = 0, const uint64_t currentId = 0) noexcept {
        if (currentId >= boxes_.size()) {
          return false;
        }
        AABB &box = boxes_.at(currentId);
        if (box.intersect(ray)) {
          if (currentId + 1 >= primitives_.size()) {
            bool res {false};
            for (MobileRT::Primitive<T> &s : primitives_.at(currentId)) {
              res |= s.intersect(intersection, ray);
            }
            return res;
          }
          const uint64_t left {currentId + (static_cast<unsigned long>(2 << depth))};
          const bool hit_left {trace(intersection, ray, depth + 1, left)};
          const bool hit_right {trace(intersection, ray, depth + 1, left + 1)};
          return hit_left || hit_right;
        }
        return false;
      }

      bool shadowTrace (::MobileRT::Intersection *const intersection, const ::MobileRT::Ray &ray, const unsigned depth = 0, const uint64_t currentId = 0) noexcept {
        if (currentId >= boxes_.size()) {
          return false;
        }
        AABB &box = boxes_.at(currentId);
        if (box.intersect(ray)) {
          const unsigned numberOfNodesDepth {static_cast<unsigned>(2 << depth)};
          if (currentId + 1 >= primitives_.size()) {
            for (MobileRT::Primitive<T> &s : primitives_.at(currentId)) {
              if (s.intersect(intersection, ray)) {
                return true;
              }
            }
            return false;
          }
          const uint64_t left {currentId + numberOfNodesDepth + 0};
          const bool hit_left {shadowTrace(intersection, ray, depth + 1, left)};
          const bool hit_right {shadowTrace(intersection, ray, depth + 1, left + 1)};
          return hit_left || hit_right;
        }
        return false;
      }
  };
}//namespace MobileRT
#endif //MOBILERT_ACCELERATORS_BVH2_HPP
