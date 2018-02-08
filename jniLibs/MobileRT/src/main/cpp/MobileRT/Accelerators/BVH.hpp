//
// Created by puscas on 27/08/17.
//

#ifndef MOBILERT_ACCELERATORS_BVH_HPP
#define MOBILERT_ACCELERATORS_BVH_HPP

#include "../Intersection.hpp"
#include "../Scene.hpp"
#include "AABB.hpp"
#include <random>

namespace MobileRT {
  static unsigned counter {0};

  template<typename T>
  class BVH final {
    public:
      ::MobileRT::AABB box_ {};
      ::std::unique_ptr<BVH> left_ {nullptr};
      ::std::unique_ptr<BVH> right_ {nullptr};
      ::std::vector<MobileRT::Primitive<T>> primitives_ {};

    public:
      explicit BVH () noexcept = default;

      explicit BVH<T> (::std::vector<MobileRT::Primitive<T>> primitives, const unsigned depth) noexcept {
        if (primitives.empty()) {
          return;
        }

        for (uint64_t i {0}; i < primitives.size(); i++) {
          AABB new_box {primitives.at(i).getAABB()};
          box_ = surroundingBox(new_box, box_);
        }

        /*static ::std::uniform_real_distribution<float> uniform_dist {0.0f, 1.0f};
        static ::std::mt19937 gen (::std::random_device {} ());
        const float randomNumber {uniform_dist (gen)};
        const int axis {static_cast<int> (3.0f * randomNumber)};*/
        const int axis {box_.getLongestAxis()};
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
        LOG("depth = ", depth, ", size = ", primitives.size(), ", divide = ", divide);

        if (primitives.size() <= 10 || depth >= 12) {
          primitives_ = primitives;
        } else {
          using Iterator = typename ::std::vector<MobileRT::Primitive<T>>::const_iterator;
          Iterator leftBegin {primitives.begin()};
          Iterator leftEnd {primitives.begin() + divide};
          Iterator rightBegin {primitives.begin() + divide};
          Iterator rightEnd {primitives.end()};

          ::std::vector<MobileRT::Primitive<T>> leftVector(leftBegin, leftEnd);
          ::std::vector<MobileRT::Primitive<T>> rightVector(rightBegin, rightEnd);

          left_ = std::make_unique<BVH> (leftVector, depth + 1);
          right_ = std::make_unique<BVH> (rightVector, depth + 1);
        }
        counter++;
      }


      explicit BVH<T> (::std::vector<MobileRT::Primitive<T>>&& primitives, const uint64_t depth) noexcept {
        if (primitives.empty()) {
          return;
        }
        const uint64_t N {primitives.size()};

        for (uint64_t i {0}; i < N; i++) {
          const AABB new_box {primitives.at(i).getAABB()};
          box_ = surroundingBox(new_box, box_);
        }
        const int axis {box_.getLongestAxis()};
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

        std::vector<AABB> boxes {};
        for (uint64_t i {0}; i < N; i++) {
          boxes.emplace_back(primitives.at(i).getAABB());
        }

        uint64_t min_SAH_idx {0};
        {
          std::vector<float> left_area {boxes.at(0).getSurfaceArea()};
          AABB left_box {};
          for (uint64_t i {0}; i < N - 1; i++) {
            left_box = surroundingBox(left_box, boxes.at(i));
            left_area.insert(left_area.end(), left_box.getSurfaceArea());
          }

          std::vector<float> right_area {boxes.at(N - 1).getSurfaceArea()};
          AABB right_box {};
          for (uint64_t i {N - 1}; i > 0; i--) {
            right_box = surroundingBox(right_box, boxes.at(i));
            right_area.insert(right_area.begin(), right_box.getSurfaceArea());
          }

          float min_SAH {std::numeric_limits<float>::max()};
          for (uint64_t i {0}; i < N - 1; i++) {
            const float SAH_left {i * left_area.at(i)};
            const float SAH_right {(N - i - 1) * right_area.at(i)};
            const float SAH {SAH_left + SAH_right};
            if (SAH < min_SAH) {
              min_SAH_idx = i;
              min_SAH = SAH;
            }
          }
        }
        LOG("depth = ", depth, ", size = ", primitives.size(), ", min_SAH_idx = ", min_SAH_idx);

        //0 primitives to left || 0 primitives to right
        if (min_SAH_idx == 0 || min_SAH_idx == N - 2) {
          primitives_ = primitives;
        } else {
          using Iterator = typename ::std::vector<MobileRT::Primitive<T>>::const_iterator;
          Iterator leftBegin {primitives.begin()};
          Iterator leftEnd {primitives.begin() + static_cast<int64_t>(min_SAH_idx) + 1};
          Iterator rightBegin {primitives.begin() + static_cast<int64_t>(min_SAH_idx) + 1};
          Iterator rightEnd {primitives.end()};

          ::std::vector<MobileRT::Primitive<T>> leftVector(leftBegin, leftEnd);
          ::std::vector<MobileRT::Primitive<T>> rightVector(rightBegin, rightEnd);

          left_ = std::make_unique<BVH> (std::move(leftVector), depth + 1);
          right_ = std::make_unique<BVH> (std::move(rightVector), depth + 1);
        }

        counter++;
      }

      BVH (const BVH &bVH) noexcept = delete;

      BVH (BVH &&bVH) noexcept = default;

      ~BVH () noexcept = default;

      BVH &operator= (const BVH &bVH) noexcept = delete;

      BVH &operator= (BVH &&bVH) noexcept = default;

      bool trace (::MobileRT::Intersection *const intersection, const ::MobileRT::Ray &ray) noexcept {
        if (box_.intersect(ray)) {
          if (left_ == nullptr) {
            bool res {false};
            for (MobileRT::Primitive<T> &s : primitives_) {
              res |= s.intersect(intersection, ray);
            }
            return res;
          }
          const bool hit_left {left_->trace(intersection, ray)};
          const bool hit_right {right_->trace(intersection, ray)};
          return hit_left || hit_right;
        }
        return false;
      }

      bool shadowTrace (::MobileRT::Intersection *intersection, const Ray &ray) noexcept {
        if (box_.intersect(ray)) {
          if (left_ == nullptr) {
            for (MobileRT::Primitive<T> &s : primitives_) {
              if (s.intersect(intersection, ray)) {
                return true;
              }
            }
            return false;
          }
          if (left_->shadowTrace(intersection, ray)) {
            return true;
          }
          return right_->shadowTrace(intersection, ray);
        }
        return false;
      }
  };
}//namespace MobileRT
#endif //MOBILERT_ACCELERATORS_BVH_HPP
