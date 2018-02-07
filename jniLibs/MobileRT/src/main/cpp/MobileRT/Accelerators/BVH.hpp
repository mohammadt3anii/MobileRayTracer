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

      explicit BVH<T> (::MobileRT::AABB sceneBounds, ::std::vector<MobileRT::Primitive<T>> primitives, const unsigned depth) noexcept {
        if (primitives.empty()) {
          return;
        }
        thread_local static ::std::uniform_real_distribution<float> uniform_dist {0.0f, 1.0f};
        thread_local static ::std::mt19937 gen (::std::random_device {} ());
        const float randomNumber {uniform_dist (gen)};
        const int axis {static_cast<int> (3.0f * randomNumber)};
        switch (axis) {
          case 0:
            std::sort(primitives.begin(), primitives.end(), [](const MobileRT::Primitive<T>&a, MobileRT::Primitive<T>& b) noexcept -> bool {
              return a.getAABB().pointMin_.x_() < b.getAABB().pointMin_.x_();
            });
            break;

          case 1:
            std::sort(primitives.begin(), primitives.end(), [](const MobileRT::Primitive<T>&a, MobileRT::Primitive<T>& b) noexcept -> bool {
              return a.getAABB().pointMin_.y_() < b.getAABB().pointMin_.y_();
            });
            break;

          default:
            std::sort(primitives.begin(), primitives.end(), [](const MobileRT::Primitive<T>&a, MobileRT::Primitive<T>& b) noexcept -> bool {
              return a.getAABB().pointMin_.z_() < b.getAABB().pointMin_.z_();
            });
            break;
        }

        using Iterator = typename ::std::vector<MobileRT::Primitive<T>>::const_iterator;
        Iterator leftBegin {};
        Iterator leftEnd {};

        Iterator rightBegin {};
        Iterator rightEnd {};

        ::MobileRT::AABB leftBox {};
        ::MobileRT::AABB rightBox {};
        leftBegin = primitives.begin();
        leftEnd = primitives.begin() + primitives.size() / 2;
        rightBegin = primitives.begin() + primitives.size() / 2;
        rightEnd = primitives.end();

        if (primitives.size() <= 10 || depth >= 12) {
          left_ = nullptr;
          right_ = nullptr;
          primitives_ = primitives;
          for (uint32_t i{0}; i < primitives.size() / 2; i++) {
            const ::MobileRT::AABB box {primitives_[i].getAABB()};
            leftBox = surroundingBox (leftBox, box);
          }
          for (uint32_t i{static_cast<uint32_t>(primitives.size()) / 2}; i < primitives_.size(); i++) {
            const ::MobileRT::AABB box {primitives_[i].getAABB()};
            rightBox = surroundingBox (rightBox, box);
          }
        } else {
          ::std::vector<MobileRT::Primitive<T>> leftVector(leftBegin, leftEnd);
          ::std::vector<MobileRT::Primitive<T>> rightVector(rightBegin, rightEnd);
          left_ = std::make_unique<BVH> (sceneBounds, leftVector, depth+1);
          right_ = std::make_unique<BVH> (sceneBounds, rightVector, depth+1);
          leftBox = left_->box_;
          rightBox = right_->box_;
        }
        box_ = surroundingBox (leftBox, rightBox);
        counter++;
      }

      BVH (const BVH &bVH) noexcept = delete;

      BVH (BVH &&bVH) noexcept = default;

      ~BVH () noexcept = default;

      BVH &operator= (const BVH &bVH) noexcept = delete;

      BVH &operator= (BVH &&bVH) noexcept = default;

      bool trace (::MobileRT::Intersection *const intersection, const ::MobileRT::Ray &ray) noexcept {
        if (box_.intersect(ray)) {
          if (left_ == nullptr || right_ == nullptr) {
            //return primitives_[0].intersect(intersection, ray);
            bool res {false};
            for (MobileRT::Primitive<T> &s : primitives_) {
              res |= s.intersect(intersection, ray);
            }
            //LOG ("size = ", primitives_.size());
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
          if (left_ == nullptr || right_ == nullptr) {
            //return primitives_[0].intersect(intersection, ray);
            for (MobileRT::Primitive<T> &s : primitives_) {
              if (s.intersect(intersection, ray)) {
                return true;
              }
            }
            return false;
          }
          if (left_->trace(intersection, ray)) {
            return true;
          }
            return right_->trace(intersection, ray);
        }
        return false;
      }
  };
}//namespace MobileRT
#endif //MOBILERT_ACCELERATORS_BVH_HPP
