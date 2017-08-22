//
// Created by Tiago on 22/08/17.
//

#include "KdTree.hpp"

using MobileRT::KdTree;

bool KdTree::trace (Intersection * /*intersection*/, const Ray & /*ray*/) const noexcept {
  return false;
}

bool KdTree::shadowTrace (Intersection * /*intersection*/, Ray && /*ray*/) const noexcept {
  return false;
}
