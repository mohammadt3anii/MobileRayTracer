//
// Created by puscas on 27/08/17.
//

#include "BVH.hpp"
#include <random>

using ::MobileRT::BVH;
using ::MobileRT::Primitive;

BVH::BVH (AABB sceneBounds, std::vector<MobileRT::Primitive<MobileRT::Sphere>> spheres) noexcept {
  LOG ("BVH");
  /*thread_local static ::std::uniform_real_distribution<float> uniform_dist {0.0f, 1.0f};
  thread_local static ::std::mt19937 gen (::std::random_device {} ());
  const float randomNumber {uniform_dist (gen)};*/
  const float randomNumber {0.5f};
  const int axis {static_cast<int> (3.0f * randomNumber)};
  switch (axis) {
    case 0:
      std::sort(spheres.begin(), spheres.end(), [](const MobileRT::Primitive<MobileRT::Sphere>&a, MobileRT::Primitive<MobileRT::Sphere>& b) -> bool {
        return a.getAABB().pointMin_.x_() < b.getAABB().pointMin_.x_();
      });
      break;

    case 1:
      std::sort(spheres.begin(), spheres.end(), [](const MobileRT::Primitive<MobileRT::Sphere>&a, MobileRT::Primitive<MobileRT::Sphere>& b) -> bool {
        return a.getAABB().pointMin_.y_() < b.getAABB().pointMin_.y_();
      });
      break;

    default:
      std::sort(spheres.begin(), spheres.end(), [](const MobileRT::Primitive<MobileRT::Sphere>&a, MobileRT::Primitive<MobileRT::Sphere>& b) -> bool {
        return a.getAABB().pointMin_.z_() < b.getAABB().pointMin_.z_();
      });
      break;
  }

  ::std::vector<MobileRT::Primitive<MobileRT::Sphere>>::const_iterator leftBegin {};
  ::std::vector<MobileRT::Primitive<MobileRT::Sphere>>::const_iterator leftEnd {};

  ::std::vector<MobileRT::Primitive<MobileRT::Sphere>>::const_iterator rightBegin {};
  ::std::vector<MobileRT::Primitive<MobileRT::Sphere>>::const_iterator rightEnd {};

  AABB leftBox {};
  AABB rightBox {};
  #define SIZE 1

  if (spheres.size() <= SIZE) {
    leftBegin = spheres.begin();
    leftEnd = spheres.end();
    rightBegin = spheres.begin();
    rightEnd = spheres.end();

    left_ = nullptr;
    right_ = nullptr;
    leftBox = spheres[0].getAABB();
    rightBox = spheres[0].getAABB();
    spheres_ = spheres;
  /*} else if (spheres.size() == 2) {
    leftBegin = spheres.begin();
    leftEnd = spheres.begin() + 1;
    rightBegin = spheres.begin() + 1;
    rightEnd = spheres.end();

    ::std::vector<MobileRT::Primitive<MobileRT::Sphere>> leftVector(leftBegin, leftEnd);
    ::std::vector<MobileRT::Primitive<MobileRT::Sphere>> rightVector(rightBegin, rightEnd);
    leftBox = spheres[0].getAABB();
    rightBox = spheres[1].getAABB();
    left_ = new BVH(sceneBounds, leftVector);
    right_ = new BVH(sceneBounds, rightVector);*/
  } else {
    leftBegin = spheres.begin();
    leftEnd = spheres.begin() + spheres.size() / 2;
    rightBegin = spheres.begin() + spheres.size() / 2;
    rightEnd = spheres.end();

    ::std::vector<MobileRT::Primitive<MobileRT::Sphere>> leftVector(leftBegin, leftEnd);
    ::std::vector<MobileRT::Primitive<MobileRT::Sphere>> rightVector(rightBegin, rightEnd);
    left_ = new BVH(sceneBounds, leftVector);
    right_ = new BVH(sceneBounds, rightVector);
    /*for (unsigned i {0}; i < left_->spheres_.size(); i++) {
      AABB box1 {left_->spheres_[i].getAABB()};
      leftBox = surroundingBox (leftBox, box1);
    }
    left_->box_ = leftBox;
    for (unsigned i {0}; i < right_->spheres_.size(); i++) {
      AABB box1 {right_->spheres_[i].getAABB()};
      rightBox = surroundingBox (rightBox, box1);
    }
    right_->box_ = rightBox;*/
    leftBox = left_->box_;
    rightBox = right_->box_;
  }
  box_ = surroundingBox (leftBox, rightBox);
}

bool BVH::trace (Intersection *intersection, const Ray &ray) noexcept {
  if (box_.intersect(ray)) {
    if (left_ == nullptr || right_ == nullptr) {
      bool res {false};
      for (MobileRT::Primitive<MobileRT::Sphere>  s : spheres_) {
        res |= s.intersect(intersection, ray);
      }
      if (res) {
        LOG ("BVH::trace res = ", res, " size = ", spheres_.size(), " KD = ", intersection->material_->Kd_.R_, " ", intersection->material_->Kd_.G_, " ", intersection->material_->Kd_.B_);
      }
      return res;
    }
    const bool hit_left {left_->trace(intersection, ray)};
    const bool hit_right {right_->trace(intersection, ray)};
    return hit_left || hit_right;
  }
  return false;
}

bool BVH::shadowTrace (Intersection *const intersection, Ray &&ray) noexcept {
  return trace(intersection, ray);
}
