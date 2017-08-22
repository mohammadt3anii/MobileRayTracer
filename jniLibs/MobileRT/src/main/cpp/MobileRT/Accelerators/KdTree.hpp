//
// Created by Tiago on 22/08/17.
//

#ifndef MOBILERT_ACCELERATORS_KDTREE_HPP
#define MOBILERT_ACCELERATORS_KDTREE_HPP

#include "../Scene.hpp"

namespace MobileRT {
  class KdTree final {
    public:
    explicit KdTree () = default;
    explicit KdTree (const KdTree &kDTree) = delete;
    explicit KdTree (KdTree &&kDTree) = delete;
    ~KdTree () = default;
    KdTree &operator= (const KdTree &kDTree) = delete;
    KdTree &operator= (KdTree &&kDTree) = default;
    bool trace (Intersection *intersection, const Ray &ray) const noexcept;
    bool shadowTrace (Intersection *intersection, Ray &&ray) const noexcept;
  };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_KDTREE_HPP
